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
			env_replace(data->env, env_line, export_token->value);
			env_replace(data->secret_env, env_line, export_token->value);
		}
		else
			env_add_back(&data->env, export_token->value);
	}
	return (0);
}

int	process_export(t_data *data, t_token *export_token)
{
	int	exit_status;

	exit_status = 0;
	if (!check_char(export_token->value))
	{
		printf("minishell: export: `%s': not a valid identifier\n",
			export_token->value);
		return (1);
	}
	if (valid_env_name(data->secret_env, export_token->value))
		exit_status = handle_assign(data, export_token);
	else
	{
		env_add_back(&data->secret_env, export_token->value);
		env_add_back(&data->env, export_token->value);
	}
	return (exit_status);
}

int	export_command(t_data *data, t_token *token)
{
	t_token	*export_token;
	int		exit_status;

	if (!token->next || token->next->type != ARG)
		return (single_export(data->env));
	export_token = token->next;
	while (export_token && export_token->type == ARG)
	{
		exit_status = process_export(data, export_token);
		export_token = export_token->next;
	}
	return (exit_status);
}
