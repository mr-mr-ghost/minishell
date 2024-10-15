/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 11:02:46 by gklimasa          #+#    #+#             */
/*   Updated: 2024/10/15 14:57:42 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* example: echo "Hello World!" | tr 'a-z' 'A-Z' | tr '!' '?'*/
/* example: ls -l | grep '.c' | wc -l*/
/* example: echo "Hello World!" | tr 'H' 'h' | tr 'e' 'E' | tr 'l' 'L'*/

int	launch_cmd_inpipe(t_data *data, t_token *cmdt)
{
	t_token	*redirt;
	int		status;
	t_token	*hdtoken;

	redirt = return_redirt(cmdt);
	if (!redirt)
	{
		status = check_launch_builtins(data, cmdt);
		if (status == -1)
			child_process(data, cmdt, NULL);
		else
			return (status);
	}
	else
	{
		hdtoken = return_1stheredoct(cmdt);
		if (is_cmd(cmdt->value, 0))
		{
			//return (handle_heredoc_builtins(data, cmdt, hdtoken));
			if (!hdtoken)
				return (redirection_wrap_builtins(data, cmdt, redirt));
			else
			{
				while (redirt && redirt->type == HEREDOC)
					redirt = return_redirt(redirt->next);
				if (!redirt)
					return (check_launch_builtins(data, cmdt));
				else
					return (redirection_wrap_builtins(data, cmdt, redirt));
			}
		}
		else
			child_process(data, cmdt, redirt);
	}
	return (0);
}

/* int	close_fd(int fd[3][2], int dst, char *heredoc)
{
	int	error;

	error = 0;
	if (!heredoc)
	{
		error = dup2(fd[dst][dst], dst);
		if (error < 0)
			err_msg(NULL, NULL, strerror(errno), 1);
		close(fd[dst][0]);
		close(fd[dst][1]);
	}
	else
	{
		if (fd[0][0] != -1 && fd[0][1] != -1)
		{
			close(fd[dst][0]);
			close(fd[dst][1]);
		}
		error = dup2(fd[2][dst], dst);
		if (error < 0)
			err_msg(NULL, NULL, strerror(errno), 1);
		close(fd[2][0]);
		close(fd[2][1]);
	}
	return (error);
} */

void	close_fd(int *fd, int dst, char *heredoc)
{
	if (!heredoc)
		dup2(fd[dst], dst);
	close(fd[0]);
	close(fd[1]);
}

int	pipe_fork(t_data *data, t_token *cmdt, int pipefd[3][2], char *heredoc)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
		return (err_msg(NULL, NULL, strerror(errno), -1));
	else if (pid == 0)
	{
		/* if ((pipefd[0][0] != -1 && pipefd[0][1] != -1) || heredoc)
			status = close_fd(pipefd, STDIN_FILENO, heredoc);
		if (heredoc)
			free(heredoc);
		if ((pipefd[1][0] != -1 && pipefd[1][1] != -1) && status == 0)
			status = close_fd(pipefd, STDOUT_FILENO, NULL);
		if (status == 0)
			status = launch_cmd_inpipe(data, cmdt); */
		if (pipefd[0][0] != -1 && pipefd[0][1] != -1)
			close_fd(pipefd[0], STDIN_FILENO, heredoc);
		if (heredoc)
		{
			dup2(pipefd[2][0], STDIN_FILENO);
			close(pipefd[2][0]);
			close(pipefd[2][1]);
			free(heredoc);
		}
		if (pipefd[1][0] != -1 && pipefd[1][1] != -1)
			close_fd(pipefd[1], STDOUT_FILENO, NULL);
		status = launch_cmd_inpipe(data, cmdt);

		rl_clear_history();
		free_tokens(data);
		free_env(data->env);
		free_env(data->secret_env);
		exit(status);
	}
	return (pid);
}

int	call_pipe(t_data *data, t_token *currentt)
{
	t_token	*nextt;
	t_token	*hdtoken;
	char	*heredoc;
	int		pipefd[3][2]; //pipefd[0] prev, pipefd[1] current, pipefd[2] heredoc
	int		status;
	pid_t	pid;

	signal_manager(sigint_handler_incmd, SA_RESTART);
	heredoc = NULL;
	pipefd[0][0] = -1;
	pipefd[0][1] = -1;
	pipefd[1][0] = -1;
	pipefd[1][1] = -1;
	pipefd[2][0] = -1;
	pipefd[2][1] = -1;
	pid = 0;
	while (currentt)
	{
		nextt = get_nth_token(currentt, count_args(currentt, PIPE));
		if (nextt && nextt->type == PIPE && nextt->next)
			nextt = nextt->next;
		else
			nextt = NULL;
		hdtoken = return_1stheredoct(currentt);
		if (hdtoken)
		{
			heredoc = get_heredoc(data, hdtoken->next->value);
			if (g_sigint) // copilot offer
			{
				if (heredoc)
					free(heredoc);
				status = 0x8200;
				break ;
			}
			if (pipe(pipefd[2]) == -1)
			{
				if (heredoc)
					free(heredoc);
				status = err_msg(NULL, NULL, strerror(errno), 1);
				break ;
			}
		}
		if (nextt && pipe(pipefd[1]) == -1) // pipe fail
		{
			if (hdtoken && heredoc)
				free(heredoc);
			status = err_msg(NULL, NULL, strerror(errno), 1);
			break ;
		}
		if (currentt->prev == NULL) // first command fork
		{
			pipefd[0][0] = -1;
			pipefd[0][1] = -1;
			status = pipe_fork(data, currentt, pipefd, heredoc);
		}
		else if (nextt) // mid command fork
			status = pipe_fork(data, currentt, pipefd, heredoc);
		else // end command fork
		{
			pipefd[1][0] = -1;
			pipefd[1][1] = -1;
			pid = pipe_fork(data, currentt, pipefd, heredoc);
		}
		if (currentt->prev != NULL)
		{	// Close the previous pipe in the parent
			close(pipefd[0][0]);
			close(pipefd[0][1]);
		}
		if (nextt)
		{	// Move the current pipe to prev_pipefd for the next iteration
			pipefd[0][0] = pipefd[1][0];
			pipefd[0][1] = pipefd[1][1];
		}
		currentt = nextt; // set next command as current command
		if (hdtoken)
		{
			close(pipefd[2][0]);
			ft_putstr_fd(heredoc, pipefd[2][1]);
			close(pipefd[2][1]);
			if (heredoc)
				free (heredoc);
			heredoc = NULL;
			hdtoken = NULL;
		}
		if (status < 0 || pid < 0) // errors from forks?
			currentt = NULL;
	}
	if (pipefd[0][0] != -1)  // Close the last pipe in the parent process
		close(pipefd[0][0]);
	if (pipefd[0][1] != -1)
		close(pipefd[0][1]);
	waitpid(pid, &status, 0);
	if (g_sigint)
		status = 0x8200;
	while (wait(NULL) > 0) // Wait for all child processes
		;
	signal_manager(sigint_handler, SA_RESTART);
	return (WEXITSTATUS(status));
}
