/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 11:53:50 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/17 16:54:59 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// returns the nth token from the list
t_token *get_nth_token(t_token *token, int n)
{
	t_token	*tmp;
	int		i;

	if (!token)
		return (NULL);
	i = 0;
	tmp = token;
	while (tmp && i < n)
	{
		i++;
		tmp = tmp->next;
	}
	if (i < n)
		return (NULL);
	return (tmp);
}

// Function to check if the command is builtin and then launch it
// Returns -1, if command not builtin
// Returns 0, if builtin command executed successfully
// Returns 1, if builtin command execution failed
int check_launch_builtins(t_data *data)
{
	t_token	*token;
	int		i;

	token = data->token;
	i = -1;
	if (ft_memcmp(token->value, "echo", ft_strlen("echo") + 1) == 0)
		i = echo_command(token);
	else if (ft_memcmp(token->value, "cd", ft_strlen("cd") + 1) == 0)
		i = cd_command(token);
	else if (ft_memcmp(token->value, "pwd", ft_strlen("pwd") + 1) == 0)
		i = pwd_command();
	else if (ft_memcmp(token->value, "export", ft_strlen("export") + 1) == 0)
		i = export_command(token, data->env);
	else if (ft_memcmp(token->value, "unset", ft_strlen("unset") + 1) == 0)
		i = unset_command(token);
	else if (ft_memcmp(token->value, "env", ft_strlen("env") + 1) == 0)
		i = env_command(token, data->env);
	else if (ft_memcmp(token->value, "exit", ft_strlen("exit") + 1) == 0)
		i = exit_command(data, token);
	return (i);
}

// Function to execute non builtin command in a child process
int	launch_nonbuiltins(char **cmd, char **envp, t_token *token)
{
	pid_t	pid;
	int		status;
	int		result;

	(void)token;
	pid = fork();
	if (pid == 0)
	{
		if (token)
		{
			result = handle_redirection(token->next, token->type);
			printf("handle_redir return: %d\n", result);
			if (result == -1)
				exit(EXIT_FAILURE);
		}
		execve(cmd[0], cmd, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("fork");
	else
	{
		result = waitpid(pid, &status, WUNTRACED); // WNOHANG no wait, res 0
		if (result == -1)
			perror("waitpid");
		else
		{
			if (WIFEXITED(status))
				printf("child exited with status %d\n", WEXITSTATUS(status));
			else if (WIFSIGNALED(status))
				printf("child terminated by signal %d\n", WTERMSIG(status));
			else if (WIFSTOPPED(status))
				printf("child stopped by signal %d\n", WSTOPSIG(status));
		}
	}
	return (0);
}

int	launch_both_cmd_types(char **cmd, int clen, char **envp, t_data *data)
{
	int	status;

	status = check_launch_builtins(data);
	if (status != -1)
		return (status);
	cmd = form_cmd(data->token, clen);
	if (!cmd)
		return (0);
	status = launch_nonbuiltins(cmd, envp, NULL);
	free_cmd(cmd);
	return (status);
}

// Function to extract commands, check if they're builtin, launch accordingly
int	process_n_exec(t_data *data, char **envp)
{
	int		status;
	t_token	*ntoken;
	char	**cmd;
	int		clen;

	cmd = NULL;
	status = 0;
	if (!data->token)
		return (0);

	clen = count_args(data->token, TRUNC);
	//printf("clen: %d\n", clen);
	ntoken = get_nth_token(data->token, clen);
	if (!ntoken)
	{
		status = launch_both_cmd_types(cmd, clen, envp, data);
		return (status);
	}
	else if ((ntoken->type == TRUNC || ntoken->type == INPUT ||
			ntoken->type == APPEND) && ntoken->next)
	{
		status = redirection_wrap_builtins(data, ntoken);
		if (status != -1)
			return (status);
		cmd = form_cmd(data->token, clen);
		if (!cmd)
			return (0);
		status = launch_nonbuiltins(cmd, envp, ntoken);
		free_cmd(cmd);
		return (status);
	}
	return (status = 0);
}
