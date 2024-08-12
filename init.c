/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 13:41:41 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/12 13:41:41 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_env(t_data *data, char **envp)
{
	int		i;
	t_env	*new;

	data->env = (t_env *)malloc(sizeof(t_env));
	data->env->value = ft_strdup(envp[0]);
	data->env->next = NULL;
	i = 1;
	new = data->env;
	while (envp[i])
	{
		new->next = (t_env *)malloc(sizeof(t_env));
		new = new->next;
		new->value = ft_strdup(envp[i]);
		new->next = NULL;
		i++;
	}
}

char	*find_env_value(t_env *env, char *key)
{
	t_env	*tmp;
	char	env_name[BUFF_SIZE];

	tmp = env;
	while (tmp)
	{
		get_env_name(env_name, tmp->value);
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

void	set_shell_lvl(t_env *env)
{
	int		lvl;
	char	*lvl_str;
	char	*lvl_value;
	char	env_name[BUFF_SIZE];

	lvl_value = find_env_value(env, "SHLVL");
	lvl = find_env_lvl(lvl_value) + 1;
	free(lvl_value);
	while (env && env->next)
	{
		get_env_name(env_name, env->value);
		if (!ft_strncmp(env_name, "SHLVL", ft_strlen(env_name)))
		{
			free(env->value);
			lvl_str = ft_itoa(lvl);
			env->value = ft_strjoin("SHLVL=", lvl_str);
			free(lvl_str);
			return ;
		}
		env = env->next;
	}
}
