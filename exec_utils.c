/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:53:50 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/16 15:05:47 by gklimasa         ###   ########.fr       */
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
int	check_launch_builtins(t_data *data, char **cmd)
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
		i = env_command(cmd, data->env); // TOFIX: works with multiple args after merge
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
	char	**cmd2;
	t_token	*token;

	cmd = NULL;
	cmd2 = NULL;
	// check it cmd1, connector and cm2
	token = data->token;
	printf("str 1: %s\n", token->value);
	cmd = ft_split(token->value, ' ');
	if (cmd == NULL || cmd[0] == NULL || cmd[0][0] == '\0')
	{
		free_cmd(cmd);
		return (0);
	}
	if (token->next == NULL || token->next->type < TRUNC)
	{
		printf("is str 2: %d\n", token->next? token->next->type : -1);
		status = check_launch_builtins(data, cmd);
		if (status == 0 || status == 1)
		{
			free_cmd(cmd);
			return (0);
		}
		status = launch_nonbuiltins(cmd, envp);
		free_cmd(cmd);
		return (status);
	}
	printf("connector: %s\n", token->next->value);
	token = token->next->next;
	if (!token)
	{
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `newline'\n", 2);
		free_cmd(cmd);
		return (0);
	}
	printf("str 3: %s\n", token->value);
	cmd2 = ft_split(token->value, ' ');
	if (cmd2 == NULL || cmd2[0] == NULL || cmd2[0][0] == '\0')
	{
		free_cmd(cmd);
		free_cmd(cmd2);
		return (0);
	}
	// TODO: handle more than 1 command
	if (token->prev->type == END)
	{
		printf("str 1: %s\n", token->prev->prev->value);
		status = check_launch_builtins(data, cmd);
		if (status == 1)
		{
			free_cmd(cmd);
			free_cmd(cmd2);
			return (1);
		}
		else if (status == 0)
		{
			status = launch_nonbuiltins(cmd2, envp);
			free_cmd(cmd);
			free_cmd(cmd2);
			return (status);
		}
		status = launch_nonbuiltins(cmd, envp);
		// TODO: check if nonbuiltin fails
		status = check_launch_builtins(data, cmd2);
		if (status == 1)
		{
			free_cmd(cmd);
			free_cmd(cmd2);
			return (1);
		}
		else if (status == 0)
		{
			free_cmd(cmd);
			free_cmd(cmd2);
			return (0);
		}
		status = launch_nonbuiltins(cmd2, envp);
		free_cmd(cmd);
		free_cmd(cmd2);
		return (status);
	}
	// TODO: handle other connectors
	free_cmd(cmd);
	free_cmd(cmd2);
	// TODO: handle more than 2 commands
	return (0);
}
