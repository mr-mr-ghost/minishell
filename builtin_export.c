/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 13:06:41 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/18 13:07:02 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	valid_env_name(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[BUFF_SIZE];

	tmp = env;
	while (tmp)
	{
		get_env_name(env_name, tmp->line);
		if (!ft_strncmp(env_name, key, ft_strlen(env_name)))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

void single_export(t_env *env)
{
	t_env	*enviro;

	enviro = env;
	while (enviro)
	{
		printf("declare -x %s\n", enviro->line);
		enviro = enviro->next;
	}
}

int export_command(t_data *data, t_token *token)
{
	t_token	*export_token;
	char	*env_line;

	if (!token->next || token->next->type != ARG)
	{
		single_export(data->env);
		return (0);
	}
	export_token = token->next;
	if (valid_env_name(data->secret_env, export_token->value))
	{
		env_line = find_env_line(data->secret_env, export_token->value);
		if (!env_line)
			return (1);
		env_add_back(&data->env, env_line);
		return (0);
	}
	env_add_back(&data->secret_env, export_token->value);
	env_add_back(&data->env, export_token->value);
	return (0);
}
