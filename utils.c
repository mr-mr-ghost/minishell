/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:38:58 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/09 16:38:58 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_data *data)
{
	t_token	*tmp;

	if (data->line)
		free(data->line);
	while (data->token)
	{
		tmp = data->token;
		data->token = data->token->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->value)
			free(tmp->value);
		if (tmp->name)
			free(tmp->name);
		if (tmp->line)
			free(tmp->line);
		free(tmp);
	}
}

bool	check_char(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalpha(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	err_msg(char *location, char *arg, char *msg, int code)
{
	if (!location && !arg)
		printf("minishell: %s\n", msg);
	else if (!arg)
		printf("minishell: %s: %s\n", location, msg);
	else
		printf("minishell: %s: '%s': %s\n", location, arg, msg);
	return (code);
}
