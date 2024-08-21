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
int	print_env_arg(t_env *env, t_token *env_token)
{
	t_env	*enviro;
	char	*line;
	int		i;

	enviro = env;
	while (enviro)
	{
		i = 0;
		line = remove_quotes(enviro->line);
		while (env_token && env_token[i].type == ARG)
		{
			if (!ft_strncmp(enviro->name, env_token[i].value,
					ft_strlen(enviro->name)))
			{
				line = remove_quotes(env_token[i].value);
				if (!line)
					return (1);
			}
			i++;
		}
		printf("%s\n", line);
		free(line);
		enviro = enviro->next;
	}
	return (0);
}

int	env_err_msg(char *arg, char *msg, int status)
{
	printf("minishell: env: %s: %s\n", arg, msg);
	return (status);
}

int	check_env_input(t_token *env_token)
{
	int	i;

	i = 0;
	while (env_token && env_token[i].type == ARG)
	{
		if (!ft_strchr(env_token[i].value, '='))
			return (env_err_msg(env_token[i].value,
					"No such file or directory", 127));
		i++;
	}
	return (0);
}

void	print_env_end(t_env *env, t_token *env_token)
{
	int	i;

	i = 0;
	while (env_token && env_token[i].type == ARG)
	{
		if (!valid_env_name(env, env_token[i].value))
			printf("%s\n", env_token[i].value);
		i++;
	}
}

int	env_command(t_data *data, t_token *token)
{
	t_token	*env_token;
	int		status;

	if (!token->next)
		return (print_env_arg(data->env, NULL));
	env_token = token->next;
	status = check_env_input(env_token);
	if (status)
		return (status);
	status = print_env_arg(data->env, env_token);
	print_env_end(data->env, env_token);
	return (status);
}
