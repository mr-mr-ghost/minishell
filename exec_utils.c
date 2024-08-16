/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:53:50 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/16 02:04:37 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(char **cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd[i])
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

// Function to check if the command is builtin and then launch it
// Returns -1, if command not builtin
// Returns 1, if builtin command executed successfully
// Returns 0, if builtin command execution failed
int	check_launch_builtins(t_data *data, char **cmd, char **envp)
{
	int	i;

	// if (!handle_redirection(cmd)) // TODO: redirection in builtins
	//	return (-1);
	i = -1;
	if (ft_memcmp(cmd[0], "echo", ft_strlen("echo") + 1) == 0)
		i = echo_command(cmd); // TOFIX: not working after merge
	else if (ft_memcmp(cmd[0], "cd", ft_strlen("cd") + 1) == 0)
		i = cd_command(cmd);
	else if (ft_memcmp(cmd[0], "pwd", ft_strlen("pwd") + 1) == 0)
		i = pwd_command(); // TOFIX: pwd loses letter d after merge if more than 1 arg
	else if (ft_memcmp(cmd[0], "export", ft_strlen("export") + 1) == 0)
		i = export_command(cmd);
	else if (ft_memcmp(cmd[0], "unset", ft_strlen("unset") + 1) == 0)
		i = unset_command(cmd);
	else if (ft_memcmp(cmd[0], "env", ft_strlen("env") + 1) == 0)
		i = env_command(cmd, envp); // TOFIX: works with multiple args after merge
	else if (ft_memcmp(cmd[0], "exit", ft_strlen("exit") + 1) == 0)
		i = exit_command(data, cmd); // TOFIX: broken after merge
	return (i);
}

void	child_process(char **cmd, char **envp)
{
	//if (!handle_redirection(cmd))		// TOFIX: not working after merge
	//	exit(EXIT_FAILURE);
	if (execve(cmd[0], cmd, envp) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

// Function to execute non builtin command in a child process
int	launch_nonbuiltins(char **cmd, char **envp)
{
	pid_t	pid;
	int		child_status;

	pid = fork();
	if (pid == 0)
		child_process(cmd, envp);
	else if (pid < 0)
		perror("fork");
	else
	{
		if (waitpid(pid, &child_status, WUNTRACED) == -1)
			perror("waitpid");
	}
	return (0);
}

// Function to extract commands, check if they're builtin, launch accordingly
int	process_n_exec(t_data *data, char **envp)
{
	int		status;
	char	**cmd;

	//printf("executing only command 1: %s\n", data->token->value);
	// TODO: handle multiple commands
	cmd = ft_split(data->token->value, ' ');
	if (cmd == NULL || cmd[0] == NULL || cmd[0][0] == '\0')
	{
		free_cmd(cmd);
		return (0);
	}
	status = check_launch_builtins(data, cmd, envp);
	if (status == 0 || status == 1)
	{
		free_cmd(cmd);
		return (0);
	}
	status = launch_nonbuiltins(cmd, envp);
	free_cmd(cmd);
	return (status);
}
