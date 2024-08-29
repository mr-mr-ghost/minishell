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

#include "minishell.h"

// example: /bin/echo "Hello World!" | /bin/tr 'a-z' 'A-Z' | /bin/tr '!' '?'
// example: /bin/ls -l | /usr/bin/grep '.c' | /bin/wc -l
// example: /bin/echo "Hello World!" | /bin/tr 'H' 'h' | /bin/tr 'e' 'E' | /bin/tr 'l' 'L' | /bin/tr 'o' 'O'

int	execute_cmd(t_data *data, t_token *cmdt, int *input_fd, int *output_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (pid);
	}
	else if (pid == 0)
	{	// Child process
		if (input_fd)
		{	// Redirect stdin if input_fd is not NULL
			dup2(input_fd[0], STDIN_FILENO);
			close(input_fd[0]);
			close(input_fd[1]);
		}
		if (output_fd)
		{	// Redirect stdout if output_fd is not NULL
			dup2(output_fd[1], STDOUT_FILENO);
			close(output_fd[0]);
			close(output_fd[1]);
		}
		launch_single_anycmd(data, cmdt);
		if (is_cmd(cmdt->value, 0))
		{
			rl_clear_history();
			free_env(data->env);
			free_env(data->secret_env);
			free_tokens(data);
			exit(EXIT_SUCCESS);
		}
	}
	return (1);
}

int	call_pipe(t_data *data, t_token *currentt)
{
	t_token	*nextt;
	int		pipefd[2];
	int		prev_pipefd[2];
	pid_t	pid;

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
			pid = execute_cmd(data, currentt, NULL, pipefd);
		else if (nextt)
			pid = execute_cmd(data, currentt, prev_pipefd, pipefd);
		else
			pid = execute_cmd(data, currentt, prev_pipefd, NULL);
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
	return (0);
}
