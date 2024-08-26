/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nonbuiltin_handling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:50:41 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/26 15:50:41 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bin_error(t_data *data, char *cmd)
{
	int	exit_status;

	exit_status = check_error(cmd);
	free_tokens(data);
	free_env(data->env);
	free_env(data->secret_env);
	exit(exit_status);
}

// Function to execute non builtin command in a child process
int	launch_nonbuiltins(t_data *data, t_token *cmdt, t_token *redirt)
{
	pid_t	pid;
	char	*bin;
	int		status;
	int		result;

	bin = NULL;
	status = 0;
	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
	{
		bin = find_bin(data->env, cmdt->value);
		if (!bin)
			bin_error(data, cmdt->value);
		child_process(data, cmdt, redirt, bin);
	}
	else
	{
		//printf("Inside parent process with PID: %d\n", getpid());
		result = waitpid(pid, &status, 0); // WNOHANG no wait, res 0
		if (result == -1)
			perror("waitpid");
		/*else
		{
			if (WIFEXITED(status))
				printf("child exited with status %d\n", WEXITSTATUS(status));
			else if (WIFSIGNALED(status))
				printf("child terminated by signal %d\n", WTERMSIG(status));
			else if (WIFSTOPPED(status))
				printf("child stopped by signal %d\n", WSTOPSIG(status));
		}*/
	}
	free(bin);
	return (WEXITSTATUS(status));
}
