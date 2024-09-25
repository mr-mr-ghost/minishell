/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 11:02:46 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/29 11:49:23 by gklimasa         ###   ########.fr       */
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

	redirt = return_redirt(cmdt);
	if (!redirt)
	{
		status = check_launch_builtins(data, cmdt);
		if (status == -1)
			child_process(data, cmdt, NULL);
		return (status);
	}
	else if (!redirt->next)
		return (err_msg(NULL, NULL,
				"syntax error near unexpected token `newline'", 2));
	else
	{
		if (redirt->type == HEREDOC)
		{
			printf("TODO: heredoc\n");
			return (status = 0);
		}
		if (is_cmd(cmdt->value, 0)
			|| (cmdt->type >= TRUNC && cmdt->type <= INPUT))
			return (redirection_wrap_builtins(data, cmdt, redirt));
		child_process(data, cmdt, redirt);
		return (0);
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
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		err_msg(NULL, NULL, strerror(errno), 1);
		return (-1);
	}
	else if (pid == 0)
	{
		if (input_fd)
			close_fd(input_fd, STDIN_FILENO);
		if (output_fd)
			close_fd(output_fd, STDOUT_FILENO);
		status = launch_cmd_inpipe(data, cmdt);
		rl_clear_history();
		free_tokens(data);
		free_env(data->env);
		free_env(data->secret_env);
		exit(status);
	}
	return (WEXITSTATUS(status));
}

int	call_pipe(t_data *data, t_token *currentt)
{
	t_token	*nextt;
	int		pipefd[2];
	int		prev_pipefd[2];
	int		status;

	status = 0;
	g_sig.in_cmd = true;
	while (currentt)
	{
		nextt = get_nth_token(currentt, count_args(currentt, PIPE));
		if (nextt && nextt->type == PIPE && nextt->next)
			nextt = nextt->next;
		else
		{
			if (nextt && nextt->type == PIPE && nextt->next == NULL)
				status = err_msg(NULL, NULL, "Unclosed pipe", 2);
			nextt = NULL;
		}
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
			status = pipe_fork(data, currentt, prev_pipefd, NULL);
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
		if (status < 0)
		{
			currentt = NULL;
			status = 1;
		}
	}
	close(prev_pipefd[0]); // Close the last pipe in the parent process
	close(prev_pipefd[1]);
	while (wait(&status) > 0) // Wait for all child processes
		;
	if (g_sig.sigint)
		return (130);
	return (WEXITSTATUS(status));
}
