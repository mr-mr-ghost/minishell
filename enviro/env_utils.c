/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 16:44:58 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/10 20:21:48 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_env_line(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[ARG_MAX];
	char	key_name[ARG_MAX];

	tmp = env;
	get_env_name(key_name, key);
	while (tmp)
	{
		get_env_name(env_name, tmp->line);
		if (!ft_strcmp(env_name, key_name))
			return (ft_strdup(tmp->line));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*find_env_name(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[ARG_MAX];
	char	key_name[ARG_MAX];

	tmp = env;
	get_env_name(key_name, key);
	while (tmp)
	{
		get_env_name(env_name, tmp->line);
		if (!ft_strcmp(env_name, key_name))
			return (ft_strdup(tmp->name));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*find_env_value(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[ARG_MAX];
	char	key_name[ARG_MAX];

	tmp = env;
	get_env_name(key_name, key);
	while (tmp)
	{
		get_env_name(env_name, tmp->line);
		if (!ft_strcmp(env_name, key_name))
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*get_env_name(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] && (ft_isalnum(src[i]) || src[i] == '_') && i < ARG_MAX - 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
