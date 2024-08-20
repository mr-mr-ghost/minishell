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

int	single_export(t_env *env)
{
	t_env	*enviro;

	enviro = env;
	while (enviro)
	{
		printf("declare -x %s\n", enviro->line);
		enviro = enviro->next;
	}
	return (0);
}

void	delete_replace(t_env **env, char *key, char *line)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, key, ft_strlen(tmp->name)))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			env_delone(tmp);
			env_add_back(env, line);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	handle_assign(t_data *data, t_token *token)
{
	t_token	*export_token;
	char	*env_line;

	export_token = token;
	if (!ft_strstr(export_token->value, "="))
	{
		env_line = find_env_line(data->secret_env, export_token->value);
		if (!env_line)
			return (1);
		env_add_back(&data->env, env_line);
	}
	else if (ft_strstr(export_token->value, "="))
	{
		if (valid_env_name(data->env, export_token->value))
		{
			env_line = find_env_name(data->env, export_token->value);
			if (!env_line)
				return (1);
			delete_replace(&data->env, env_line, export_token->value);
			delete_replace(&data->secret_env, env_line, export_token->value);
		}
		else
			env_add_back(&data->env, export_token->value);
	}
	return (0);
}

int	export_command(t_data *data, t_token *token)
{
	t_token	*export_token;

	if (!token->next || token->next->type != ARG)
		return (single_export(data->env));
	export_token = token->next;
	if (valid_env_name(data->secret_env, export_token->value))
		return (handle_assign(data, export_token));
	else
	{
		env_add_back(&data->secret_env, export_token->value);
		env_add_back(&data->env, export_token->value);
	}
	return (0);
}
