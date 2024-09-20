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

#include "../minishell.h"

int	init_env(t_data *data, char **envp)
{
	data->env = create_env_list(data->env, envp);
	data->secret_env = create_env_list(data->secret_env, envp);
	if (!data->env || !data->secret_env)
		return (err_msg(NULL, NULL,
				"Environment memory allocation failure", 1));
	return (0);
}

t_env	*create_env_list(t_env *env, char **envp)
{
	int		i;

	env = env_new(envp[0]);
	if (!env)
		return (NULL);
	i = 1;
	while (envp[i])
	{
		env_add_back(&env, envp[i]);
		i++;
	}
	set_shell_lvl(env);
	return (env);
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
	else
		return (ft_strdup(""));
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

	lvl_value = find_env_line(env, "SHLVL");
	lvl = find_env_lvl(lvl_value) + 1;
	free(lvl_value);
	while (env && env->next)
	{
		get_env_name(env_name, env->line);
		if (!ft_strncmp(env_name, "SHLVL", ft_strlen(env_name)))
		{
			free(env->line);
			free(env->value);
			lvl_str = ft_itoa(lvl);
			env->line = ft_strjoin("SHLVL=", lvl_str);
			env->value = ft_strdup(lvl_str);
			free(lvl_str);
			return ;
		}
		env = env->next;
	}
}
