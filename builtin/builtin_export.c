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

#include "../minishell.h"

int	single_export(t_env *env)
{
	t_env	*enviro;
	char	*env_line;

	enviro = env;
	while (enviro)
	{
		env_line = add_quotes_var(enviro->line);
		printf("declare -x %s\n", env_line);
		free(env_line);
		enviro = enviro->next;
	}
	return (0);
}

int	handle_assign(t_data *data, t_token *token)
{
	char	*env_line;

	if (ft_strstr(token->value, "="))
	{
		env_line = find_env_name(data->env, token->value);
		if (!env_line)
			return (1);
		env_replace(data->secret_env, env_line, token->value);
		env_replace(data->env, env_line, token->value);
	}
	else
	{
		env_line = find_env_line(data->secret_env, token->value);
		if (!env_line)
			return (1);
		if (valid_env_name(data->env, token->value))
		{
			env_replace(data->env, token->value, env_line);
			env_replace(data->secret_env, token->value, env_line);
		}
		else if (valid_env_name(data->secret_env, token->value))
			env_add_back(&data->env, env_line);
	}
	free(env_line);
	return (0);
}

int	process_export(t_data *data, t_token *export_token)
{
	int		exit_status;
	char	*env_line;

	exit_status = 0;
	if (!check_char(export_token->value))
		return (err_msg("export", export_token->value,
				"not a valid identifier", 1));
	if (valid_env_name(data->secret_env, export_token->value))
		exit_status = handle_assign(data, export_token);
	else
	{
		env_line = ft_strdup(export_token->value);
		if (!env_line)
			return (1);
		env_add_back(&data->secret_env, env_line);
		env_add_back(&data->env, env_line);
		free(env_line);
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
