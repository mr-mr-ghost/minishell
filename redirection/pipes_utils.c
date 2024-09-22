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

// example: /bin/echo "Hello World!" | /bin/tr 'a-z' 'A-Z' | /bin/tr '!' '?'
// example: /bin/ls -l | /usr/bin/grep '.c' | /bin/wc -l
// example: /bin/echo "Hello World!" | /bin/tr 'H' 'h' | /bin/tr 'e' 'E' | /bin/tr 'l' 'L' | /bin/tr 'o' 'O'
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
	else if (!redirt->next) /* redirect sign without next string*/
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

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (pid);
	}
	else if (pid == 0)
	{	/* Child process*/
		if (input_fd) /* Redirect stdin if input_fd is not NULL*/
			close_fd(input_fd, STDIN_FILENO);
		if (output_fd) /* Redirect stdout if output_fd is not NULL*/
			close_fd(output_fd, STDOUT_FILENO);
		launch_cmd_inpipe(data, cmdt);
		rl_clear_history();
		free_tokens(data);
		free_env(data->env);
		free_env(data->secret_env);
		exit(EXIT_FAILURE);
	}
	return (1);
}

int	call_pipe(t_data *data, t_token *currentt)
{
	t_token	*nextt;
	int		pipefd[2];
	int		prev_pipefd[2];
	pid_t	pid;

	g_sig.in_cmd = true;
	while (currentt)
	{
		nextt = get_nth_token(currentt, count_args(currentt, PIPE));
		if (nextt && nextt->type == PIPE && nextt->next)
			nextt = nextt->next;
		else
		{
			if (nextt && nextt->type == PIPE && nextt->next == NULL)
				ft_putstr_fd("no command after pipe\n", 2);
			nextt = NULL;
		}
		if (nextt && pipe(pipefd) == -1)
		{
			perror("pipe");
			break ;
		}
		if (currentt->prev == NULL)
			pid = pipe_fork(data, currentt, NULL, pipefd);
		else if (nextt)
			pid = pipe_fork(data, currentt, prev_pipefd, pipefd);
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
		if (pid < 0)
			currentt = NULL;
	}
	close(prev_pipefd[0]); // Close the last pipe in the parent process
	close(prev_pipefd[1]);
	while (wait(NULL) > 0) // Wait for all child processes
		;
	if (g_sig.sigint)
		return (130);
	return (0);
}
