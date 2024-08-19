/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 16:44:58 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/15 16:45:06 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_env_line(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[BUFF_SIZE];

	tmp = env;
	while (tmp)
	{
		get_env_name(env_name, tmp->line);
		if (!ft_strncmp(env_name, key, ft_strlen(env_name)))
			return (ft_strdup(tmp->line));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*find_env_name(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[BUFF_SIZE];

	tmp = env;
	while (tmp)
	{
		get_env_name(env_name, tmp->line);
		if (!ft_strncmp(env_name, key, ft_strlen(env_name)))
			return (ft_strdup(env_name));
		tmp = tmp->next;
	}
	return (NULL);
}

char	*find_env_value(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[BUFF_SIZE];

	tmp = env;
	while (tmp)
	{
		get_env_name(env_name, tmp->line);
		if (!ft_strncmp(env_name, key, ft_strlen(env_name)))
			return (ft_strdup(tmp->value));
		tmp = tmp->next;
	}
	return (NULL);
}

int	find_env_lvl(char *env_value)
{
	int		i;
	char	*lvl_str;

	i = 0;
	while (env_value[i] && env_value[i] != '=')
		i++;
	if (env_value[i] == '=')
		i++;
	lvl_str = ft_strdup(env_value + i);
	i = ft_atoi(lvl_str);
	free(lvl_str);
	return (i);
}

char	*get_env_name(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] && src[i] != '=')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

t_env	*env_new(char *value)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->line = ft_strdup(value);
	new->name = set_env_name(value);
	new->value = set_env_value(value);
	new->next = NULL;
	return (new);
}

void	env_add_back(t_env **env, char *value)
{
	t_env	*new;
	t_env	*tmp;

	new = (t_env *)malloc(sizeof(t_env));
	new->line = ft_strdup(value);
	new->name = set_env_name(value);
	new->value = set_env_value(value);
	new->next = NULL;
	if (*env == NULL)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
