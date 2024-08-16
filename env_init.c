/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
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
	data->env->line = ft_strdup(envp[0]);
	data->env->next = NULL;
	i = 1;
	new = data->env;
	while (envp[i])
	{
		new->next = (t_env *)malloc(sizeof(t_env));
		new = new->next;
		new->line = ft_strdup(envp[i]);
		new->name = set_env_name(envp[i]);
		new->value = set_env_value(envp[i]);
		new->next = NULL;
		i++;
	}
	set_shell_lvl(data->env);
}

char	*set_env_value(char *line)
{
	char	*new_line;
	char	env_value[BUFF_SIZE];
	int		i;
	int		j;

	new_line = ft_strdup(line);
	i = 0;
	while (new_line[i] && new_line[i] != '=')
		i++;
	if (new_line[i] == '=')
		i++;
	j = 0;
	while (new_line[i])
		env_value[j++] = new_line[i++];
	env_value[j] = '\0';
	free(new_line);
	return (ft_strdup(env_value));
}

char	*set_env_name(char *line)
{
	char	*new_line;
	char	env_name[BUFF_SIZE];

	new_line = ft_strdup(line);
	get_env_name(env_name, new_line);
	free(new_line);
	return (ft_strdup(env_name));
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
		get_env_name(env_name, env->line);
		if (!ft_strncmp(env_name, "SHLVL", ft_strlen(env_name)))
		{
			free(env->line);
			lvl_str = ft_itoa(lvl);
			env->line = ft_strjoin("SHLVL=", lvl_str);
			free(lvl_str);
			return ;
		}
		env = env->next;
	}
}
