/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:53:50 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/14 23:44:28 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to check if the command is builtin and then execute it
// Returns -1, if command not builtin
// Returns 1, if builtin command executed successfully
// Returns 0, if builtin command execution failed
int	check_launch_builtins(t_data *data, char **envp)
{
	int	i;

	/* if (!handle_redirection(data->cmd)) // TODO: redirection in builtins
		return (-1); */
	i = -1;
	if (ft_memcmp(data->cmd[0], "echo", ft_strlen("echo") + 1) == 0)
		i = echo_command(data->cmd); //TODO
	else if (ft_memcmp(data->cmd[0], "cd", ft_strlen("cd") + 1) == 0)
		i = cd_command(data->cmd); //TODO
	else if (ft_memcmp(data->cmd[0], "pwd", ft_strlen("pwd") + 1) == 0)
		i = pwd_command();
	else if (ft_memcmp(data->cmd[0], "export", ft_strlen("export") + 1) == 0)
		i = export_command(data->cmd); //TODO
	else if (ft_memcmp(data->cmd[0], "unset", ft_strlen("unset") + 1) == 0)
		i = unset_command(data->cmd); // TODO
	else if (ft_memcmp(data->cmd[0], "env", ft_strlen("env") + 1) == 0)
		i = env_command(data->cmd, envp);
	else if (ft_memcmp(data->cmd[0], "exit", ft_strlen("exit") + 1) == 0)
		i = exit_command(data);
	return (i);
}

void	child_process(t_data *data, char **envp)
{
	if (!handle_redirection(data->cmd))
		exit(EXIT_FAILURE);
	if (execve(data->cmd[0], data->cmd, envp) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

// Function to execute commands
int	execute_command(t_data *data, char **envp)
{
	pid_t	pid;
	int		status;
	int		i;

	i = check_launch_builtins(data, envp);
	if (i == 0 || i == 1)
		return (i);
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
