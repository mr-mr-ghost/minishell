/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:53:50 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/14 17:17:18 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process(t_data *data, char **envp)
{
	if (!handle_redirection(data->cmd)) // TODO: adjust redirs for builtins
		exit(EXIT_FAILURE);
	if (ft_memcmp(data->cmd[0], "echo", ft_strlen("echo") + 1) == 0)
		echo_command(data->cmd); //TODO
	else if (ft_memcmp(data->cmd[0], "cd", ft_strlen("cd") + 1) == 0)
		cd_command(data->cmd); //TODO
	else if (ft_memcmp(data->cmd[0], "pwd", ft_strlen("pwd") + 1) == 0)
		pwd_command();
	else if (ft_memcmp(data->cmd[0], "export", ft_strlen("export") + 1) == 0)
		export_command(data->cmd); //TODO
	else if (ft_memcmp(data->cmd[0], "unset", ft_strlen("unset") + 1) == 0)
		unset_command(data->cmd); // TODO
	else if (ft_memcmp(data->cmd[0], "env", ft_strlen("env") + 1) == 0)
		env_command(envp);
	else
	{
		if (execve(data->cmd[0], data->cmd, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}

// Function to execute commands
int	execute_command(t_data *data, char **envp)
{
	pid_t	pid;
	int		status;

	if (ft_memcmp(data->cmd[0], "exit", ft_strlen("exit") + 1) == 0)
		return (0);
	pid = fork();
	if (pid == 0)
		child_process(data, envp);
	else if (pid < 0)
		perror("fork");
	else // parent process
	{ //WUNTRACED vs 0 ?
		if (waitpid(pid, &status, WUNTRACED) == -1)
			perror("waitpid");
	}
	return (1);
}
