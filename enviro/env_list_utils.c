/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:00:12 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/02 13:23:22 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	env_delone(t_env *env)
{
	if (env->line)
		free(env->line);
	if (env->name)
		free(env->name);
	if (env->value)
		free(env->value);
	free(env);
}

void	env_replace(t_env *env, char *name, char *line)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, name, ft_strlen(tmp->name)))
		{
			if (tmp->line)
				free(tmp->line);
			tmp->line = ft_strdup(line);
			if (tmp->name)
				free(tmp->name);
			tmp->name = set_env_name(line);
			if (tmp->value)
				free(tmp->value);
			tmp->value = set_env_value(line);
			return ;
		}
		tmp = tmp->next;
	}
	if (!tmp)
		env_add_back(&env, line);
}
