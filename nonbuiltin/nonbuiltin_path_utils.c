/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nonbuiltin_path_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:52:11 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/26 15:52:11 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_path(char *path, char *cmd)
{
	char	*tmp;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

char	*check_dir(char *bin, char *cmd)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*path;

	dir = opendir(bin);
	if (!dir)
		return (NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (!ft_strcmp(entry->d_name, cmd))
		{
			path = join_path(bin, cmd);
			closedir(dir);
			return (path);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (NULL);
}

char	**bin_path(t_env *env)
{
	char	**path;
	char	*tmp;

	tmp = find_env_value(env, "PATH");
	if (!tmp)
		return (NULL);
	path = ft_split(tmp, ':');
	free(tmp);
	return (path);
}

char	*find_bin(t_env *env, char *cmd)
{
	int		i;
	char	**bin;
	char	*path;

	if (ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));
	i = 0;
	bin = bin_path(env);
	if (!bin)
		return (NULL);
	while (bin[i])
	{
		path = check_dir(bin[i], cmd);
		if (path)
		{
			free_array(bin);
			return (path);
		}
		i++;
	}
	free_array(bin);
	return (NULL);
}
