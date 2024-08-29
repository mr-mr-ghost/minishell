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

	if (!path)
		return (NULL);
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
	char	*line;
	char	*tmp;
	char	*prompt;
	char	*env_path;

	line = ft_strdup("Minishell: ~");
	env_path = find_env_value(env, "PWD");
	env_path = shorten_path(env_path);
	if (env_path)
	{
		tmp = ft_strjoin(line, env_path);
		free(line);
		free(env_path);
		prompt = ft_strjoin(tmp, "> ");
		free(tmp);
	}
	else
		prompt = ft_strjoin(line, "> ");
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
	{
		g_sig.exit_status = 130;
		return (NULL);
	}
	new_line = ft_strdup(line);
	free(line);
	if (!line)
		return (NULL);
	return (line);
}
