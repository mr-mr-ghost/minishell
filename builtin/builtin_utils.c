/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:08:39 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/02 12:39:50 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	valid_env_name(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[ARG_MAX];
	char	key_name[ARG_MAX];

	if (!env || !key)
		return (false);
	tmp = env;
	get_env_name(key_name, key);
	while (tmp)
	{
		get_env_name(env_name, tmp->line);
		if (!ft_strcmp(env_name, key_name))
			return (true);
		tmp = tmp->next;
	}
	return (false);
}

char	*add_quotes_var(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = malloc(ft_strlen(line) + 3);
	if (!new_line)
		return (NULL);
	while (line[i] && line[i] != '=')
		new_line[j++] = line[i++];
	if (line[i] == '\0')
	{
		new_line[j] = '\0';
		return (new_line);
	}
	new_line[j++] = line[i++];
	if (line[i] != '\"')
		new_line[j++] = '\"';
	while (line[i])
		new_line[j++] = line[i++];
	if (line[i] != '\"' && line[i - 1] != '\"')
		new_line[j++] = '\"';
	new_line[j] = '\0';
	return (new_line);
}

bool	check_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}
