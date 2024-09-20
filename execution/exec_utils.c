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

#include "../minishell.h"

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

int	is_cmd(char *line, int i)
{
	const char	*cmds[] = {"echo", "cd", "pwd",
		"export", "unset", "env", "exit"};
	const int	lengths[] = {4, 2, 3, 6, 5, 3, 4};
	const int	num_cmds = sizeof(cmds) / sizeof(cmds[0]);
	int			j;

	j = 0;
	while (j < num_cmds)
	{
		if (select_cmp(line, cmds[j], i, lengths[j])
			&& ft_strchr("><|;\"\' \0", line[i + lengths[j]]))
			return (lengths[j]);
		j++;
	}
	return (0);
}

/* Function to check if the command is builtin and then launch it*/
/* Returns -1, if command not builtin*/
/* Returns 1, if builtin command executed successfully*/
/* Returns 0, if builtin command execution failed*/
int	check_launch_builtins(t_data *data, t_token *token)
{
	int	i;

	i = -1;
	if (!ft_memcmp(token->value, "echo", ft_strlen("echo") + 1))
		i = echo_command(token);
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
	else if (ft_strchr(token->value, '='))
		i = handle_declaration(data->secret_env, token);
	return (i);
}
