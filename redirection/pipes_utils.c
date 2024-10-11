/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 11:02:46 by gklimasa          #+#    #+#             */
/*   Updated: 2024/10/11 13:45:57 by gklimasa         ###   ########.fr       */
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
		return (status);
	}
	else
	{
		hdtoken = return_1stheredoct(cmdt);
		if (is_cmd(cmdt->value, 0))
		{
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
		{
			/* if (hdtoken)
				return (handle_heredoc(data, cmdt, hdtoken));
			else */
			child_process(data, cmdt, redirt);
			return (0);
		}
	}
}

void	close_fd(int *fd, int dst)
{
	dup2(fd[dst], dst);
	close(fd[0]);
	close(fd[1]);
}

int	pipe_fork(t_data *data, t_token *cmdt, int pipefd[2][2], char *heredoc)
{
	pid_t	pid;
	int		status;

	(void)heredoc;
	pid = fork();
	if (pid < 0)
		return (err_msg(NULL, NULL, strerror(errno), -1));
	else if (pid == 0)
	{
		if (pipefd[0][0] != -1 && pipefd[0][1] != -1)
			close_fd(pipefd[0], STDIN_FILENO);
		if (pipefd[1][0] != -1 && pipefd[1][1] != -1)
			close_fd(pipefd[1], STDOUT_FILENO);

		if (heredoc)
			free(heredoc);

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
	int		pipefd[2][2]; // pipefd[0] - previous pipe, pipefd[1] - current pipe
	//int		pipefd[2];
	//int		prev_pipefd[2];
	int		status;
	pid_t	pid;

	signal_manager(sigint_handler_incmd, SA_RESTART);
	heredoc = NULL;
	pid = 0;
	while (currentt)
	{
		// check for current command's heredoc
		hdtoken = return_1stheredoct(currentt);

		// get next command
		nextt = get_nth_token(currentt, count_args(currentt, PIPE));
		if (nextt && nextt->type == PIPE && nextt->next)
			nextt = nextt->next;
		else
			nextt = NULL;

		// if heredoc here, replace pipe with heredoc?

		if (hdtoken)
			heredoc = get_heredoc(data, hdtoken->next->value);

		// pipe fail
		if (nextt && pipe(pipefd[1]) == -1)
		{
			if (hdtoken && heredoc)
				free(heredoc);
			status = err_msg(NULL, NULL, strerror(errno), 1);
			break ;
		}

		// put heredoc to the pipe
		if (hdtoken && !is_cmd(currentt->value, 0))
			ft_putstr_fd(heredoc, pipefd[1][1]);

		// make forks for each command/pipe
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
		/* if (currentt->prev == NULL) // first command fork
			status = pipe_fork(data, currentt, NULL, pipefd[1], heredoc);
		else if (nextt) // mid command fork
			status = pipe_fork(data, currentt, pipefd[0], pipefd[1], heredoc);
		else // end command fork
			pid = pipe_fork(data, currentt, pipefd[0], NULL, heredoc); */

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
			if (heredoc)
				free (heredoc);
			heredoc = NULL;
			hdtoken = NULL;
		}
		if (status < 0 || pid < 0) // errors from forks?
			currentt = NULL;
	}
	close(pipefd[0][0]); // Close the last pipe in the parent process
	close(pipefd[0][1]);
	waitpid(pid, &status, 0);
	if (g_sigint)
		status = 0x8200;
	while (wait(NULL) > 0) // Wait for all child processes
		;
	signal_manager(sigint_handler, SA_RESTART);
	return (WEXITSTATUS(status));
}
