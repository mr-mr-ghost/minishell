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
int	check_launch_builtins(t_data *data, t_token *token)
{
	int	i;

	i = 127;
	if (!ft_memcmp(token->value, "echo", ft_strlen("echo") + 1))
		i = echo_command(data, token);
	else if (!ft_memcmp(token->value, "cd", ft_strlen("cd") + 1))
		i = cd_command(data, token);
	else if (!ft_memcmp(token->value, "pwd", ft_strlen("pwd") + 1))
		i = pwd_command();
	else if (!ft_memcmp(token->value, "export", ft_strlen("export") + 1))
		i = export_command(data, token);
	else if (!ft_memcmp(token->value, "unset", ft_strlen("unset") + 1))
		i = unset_command(data, token);
	else if (!ft_memcmp(token->value, "env", ft_strlen("env") + 1))
		i = env_command(data, token);
	else if (!ft_memcmp(token->value, "exit", ft_strlen("exit") + 1))
		i = exit_command(data, token);
	return (i);
}

int	handle_declaration(t_env *secret_env, t_token *token)
{
	t_token	*tmp;
	char	*name;
	char	*value;

	tmp = token;
	while (tmp && (!tmp->prev || tmp->type == ARG))
	{
		value = add_quotes_var(tmp->value);
		if (!value)
			return (1);
		if (valid_env_name(secret_env, value))
		{
			name = find_env_name(secret_env, value);
			if (!name)
				return (1);
			env_replace(secret_env, name, value);
			free(name);
		}
		else if (check_char(value))
			env_add_back(&secret_env, value);
		free(value);
		tmp = tmp->next;
	}
	return (0);
}
