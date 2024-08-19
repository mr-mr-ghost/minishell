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
int check_launch_builtins(t_data *data, t_token *token, char **envp)
{
	int	i;

	// if (!handle_redirection(cmd)) // TODO: redirection in builtins
	//	return (-1);
	(void)envp;
	i = -1;
	if (ft_memcmp(token->value, "echo", ft_strlen("echo") + 1) == 0)
		i = echo_command(token, data->env);
	else if (ft_memcmp(token->value, "cd", ft_strlen("cd") + 1) == 0)
		i = cd_command(token);
	else if (ft_memcmp(token->value, "pwd", ft_strlen("pwd") + 1) == 0)
		i = pwd_command();
	else if (ft_memcmp(token->value, "export", ft_strlen("export") + 1) == 0)
		i = export_command(data, token);
	else if (ft_memcmp(token->value, "unset", ft_strlen("unset") + 1) == 0)
		i = unset_command(data, token);
	else if (ft_memcmp(token->value, "env", ft_strlen("env") + 1) == 0)
		i = env_command(token, data->env);
	else if (ft_memcmp(token->value, "exit", ft_strlen("exit") + 1) == 0)
		i = exit_command(data, token);
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
	t_token	*token;

	// TODO: handle multiple commands
	if (!data->token)
		return (0);
	token = data->token;
	status = -1;
	if (token->type == CMD && ft_strnstr(token->value, "=", ft_strlen(token->value)))
		env_add_back(&data->secret_env, token->value);
	else if (token->type == CMD)
		status = check_launch_builtins(data, token, envp);
	if (status == 0 || status == 1)
		return (0);
//	status = launch_nonbuiltins(cmd, envp);
	return (status);
}
