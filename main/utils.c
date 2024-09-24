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

#include "../minishell.h"

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

	if (!str || !str[0])
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	err_msg(char *location, char *arg, char *msg, int code)
{
	ft_putstr_fd("minishell: ", 2);
	if (location)
	{
		ft_putstr_fd(location, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': ", 2);
	}
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	return (code);
}
