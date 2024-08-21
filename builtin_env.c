/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 13:48:33 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/18 13:48:35 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* env_command: prints all the environment variables			*/
/* if 1 str - prints env vars, returns 1						*/
int	print_env(t_env *env)
{
	t_env	*enviro;
	char	*line;
	int		flag;

	enviro = env;
	while (enviro)
	{
		flag = 0;
		if (enviro->value && (ft_strchr(enviro->value, '\"')
				|| ft_strchr(enviro->value, '\'')))
		{
			line = remove_quotes(enviro->line);
			if (!line)
				return (1);
			flag = 1;
		}
		else
			line = enviro->line;
		printf("%s\n", line);
		if (flag)
			free(line);
		enviro = enviro->next;
	}
	return (0);
}

int	handle_env(t_data *data, t_token *token)
{
	t_token	*env_token;
	char	*env_line;

	env_token = token;
	if (valid_env_name(data->env, env_token->value))
	{
		env_line = find_env_name(data->env, env_token->value);
		if (!env_line)
			return (1);
		env_replace(data->env, env_line, env_token->value);
		env_replace(data->secret_env, env_line, env_token->value);
	}
	else
	{
		env_add_back(&data->env, env_token->value);
		env_add_back(&data->secret_env, env_token->value);
	}
	return (0);
}

int	env_command(t_data *data, t_token *token)
{
	if (!token->next)
		return (print_env(data->env));
	if (ft_strstr(token->next->value, "="))
	{
		if (!handle_env(data, token->next))
			return (print_env(data->env));
	}
	else
		printf("env: %s: No such file or directory\n", token->next->value);
	return (1);
}
