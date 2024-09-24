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
		if (!env_line)
			return (err_msg("export", NULL, "Memory allocation failure", 1));
		printf("declare -x %s\n", env_line);
		free(env_line);
		enviro = enviro->next;
	}
	return (0);
}

int	handle_assign(t_data *data, t_token *token)
{
	char	*env_line;

	if (ft_strchr(token->value, '='))
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
	if (!check_char(export_token->value))
		return (err_msg("export", export_token->value,
				"not a valid identifier", 1));
	if (valid_env_name(data->secret_env, export_token->value))
		return (handle_assign(data, export_token));
	else
	{
		env_add_back(&data->secret_env, export_token->value);
		env_add_back(&data->env, export_token->value);
	}
	return (0);
}

int	export_command(t_data *data, t_token *token)
{
	t_token	*export_token;
	bool	error;
	int		exit_status;

	if (!token->next || token->next->type >= TRUNC)
		return (single_export(data->env));
	export_token = token->next;
	error = false;
	while (export_token && export_token->type <= ARG)
	{
		exit_status = process_export(data, export_token);
		if (exit_status)
			error = true;
		export_token = export_token->next;
	}
	if (error)
		return (1);
	return (0);
}
