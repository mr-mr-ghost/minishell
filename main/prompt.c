/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:13:29 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/27 12:13:29 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*shorten_path(char *path)
{
	int		slash_index;
	int		slash_count;
	char	*line;
	int		i;

	if (!path || !ft_strcmp(path, "/"))
		return (NULL);
	else if (!ft_strchr(path, '/'))
		return (ft_strdup(path));
	slash_count = 0;
	i = ft_strlen(path) - 1;
	while (i >= 0)
	{
		if (path[i] == '/')
		{
			slash_count++;
			if (slash_count <= 2)
				slash_index = i;
		}
		i--;
	}
	line = ft_substr(path, slash_index, ft_strlen(path) - slash_index);
	free(path);
	return (line);
}

char	*generate_prompt(t_env *env)
{
	char	*tmp;
	char	*prompt;
	char	*env_path;

	env_path = shorten_path(find_env_value(env, "PWD"));
	if (env_path)
	{
		if ((!ft_strchr(env_path, '/') || !ft_strncmp(env_path, "/home", 5)))
			tmp = ft_strjoin("\001\033[1;34m\002Minishell:\001\033[0m\002 ",
					env_path);
		else
			tmp = ft_strjoin("\001\033[1;34m\002Minishell:\001\033[0m\002 ~",
					env_path);
		free(env_path);
		prompt = ft_strjoin(tmp, "> ");
		free(tmp);
	}
	else
		prompt = ft_strdup("\001\033[1;34m\002Minishell:\001\033[0m\002 > ");
	return (prompt);
}

char	*read_line(t_env *env)
{
	char	*line;
	char	*prompt;
	char	*new_line;

	prompt = generate_prompt(env);
	if (!prompt)
		return (NULL);
	line = readline(prompt);
	free(prompt);
	if (!line)
		return (NULL);
	new_line = ft_strdup(line);
	free(line);
	if (!new_line)
		return (NULL);
	return (new_line);
}
