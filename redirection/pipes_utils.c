/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 11:02:46 by gklimasa          #+#    #+#             */
/*   Updated: 2024/10/08 16:55:34 by gklimasa         ###   ########.fr       */
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
		hdtoken = return_1stheredoct(cmdt); // should check if theres eof sign?
		if (is_cmd(cmdt->value, 0))
		{
			if (hdtoken)
				return (handle_heredoc_builtins(data, cmdt, hdtoken));
			else
				return (redirection_wrap_builtins(data, cmdt, redirt));
		}
		else
		{
			if (hdtoken)
				return (handle_heredoc(data, cmdt, hdtoken));
			else
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

int	pipe_fork(t_data *data, t_token *cmdt, int *input_fd, int *output_fd)
{
	pid_t	pid;
	t_token	*redirt;
	t_token	*hdtoken;
	char	*heredoc;
	int		status;

	pid = fork();
	if (pid < 0)
		return (err_msg(NULL, NULL, strerror(errno), -1));
	else if (pid == 0)
	{
		status = EXIT_FAILURE;
		redirt = return_redirt(cmdt);
		// getting heredoc before replacing stdin/out
		hdtoken = return_1stheredoct(cmdt);
		if (hdtoken)
			heredoc = get_heredoc(data, hdtoken->next->value);
		// replacing stdin/out
		if (input_fd)
			close_fd(input_fd, STDIN_FILENO);
		if (output_fd)
			close_fd(output_fd, STDOUT_FILENO);

		// launching commands accordingly
		// status = launch_cmd_inpipe(data, cmdt);
		if (is_cmd(cmdt->value, 0))
		{
			if (heredoc)
				free(heredoc);
			if (!redirt)
				status = check_launch_builtins(data, cmdt);
			else if (!heredoc)
				status = redirection_wrap_builtins(data, cmdt, redirt);
			else
			{
				while (redirt && redirt->type == HEREDOC)
					redirt = return_redirt(redirt->next);
				if (!redirt)
					status = check_launch_builtins(data, cmdt);
				else
					status = redirection_wrap_builtins(data, cmdt, redirt);
			}
		}
		else
			child_process(data, cmdt, redirt);
		// cleaning
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
	int		pipefd[2];
	int		prev_pipefd[2];
	int		status;
	pid_t	pid;

	signal_manager(sigint_handler_incmd, SA_RESTART);
	pid = 0;
	while (currentt)
	{
		nextt = get_nth_token(currentt, count_args(currentt, PIPE));
		if (nextt && nextt->type == PIPE && nextt->next)
			nextt = nextt->next;
		else
			nextt = NULL;
		if (nextt && pipe(pipefd) == -1)
		{
			status = err_msg(NULL, NULL, strerror(errno), 1);
			break ;
		}
		if (currentt->prev == NULL)
			status = pipe_fork(data, currentt, NULL, pipefd);
		else if (nextt)
			status = pipe_fork(data, currentt, prev_pipefd, pipefd);
		else
			pid = pipe_fork(data, currentt, prev_pipefd, NULL);
		if (currentt->prev != NULL)
		{	// Close the previous pipe in the parent
			close(prev_pipefd[0]);
			close(prev_pipefd[1]);
		}
		if (nextt)
		{	// Move the current pipe to prev_pipefd for the next iteration
			prev_pipefd[0] = pipefd[0];
			prev_pipefd[1] = pipefd[1];
		}
		currentt = nextt;
		if (status < 0 || pid < 0)
			currentt = NULL;
	}
	close(prev_pipefd[0]); // Close the last pipe in the parent process
	close(prev_pipefd[1]);
	waitpid(pid, &status, 0);
	if (g_sigint)
		status = 0x8200;
	while (wait(NULL) > 0) // Wait for all child processes
		;
	signal_manager(sigint_handler, SA_RESTART);
	return (WEXITSTATUS(status));
}
