/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:08:39 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/26 12:08:39 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	valid_env_name(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[BUFF_SIZE];

	if (!env || !key)
		return (false);
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

char	*remove_quotes(char *line)
{
	int		i;
	int		j;
	char	*new_line;

	i = 0;
	j = 0;
	new_line = (char *)malloc(sizeof(char) * (ft_strlen(line) + 1));
	if (!new_line)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
			i++;
		else
			new_line[j++] = line[i++];
	}
	new_line[j] = '\0';
	return (new_line);
}

char	*add_quotes_var(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = (char *)malloc(sizeof(char) * (ft_strlen(line) + 3));
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
