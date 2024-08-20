/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:34 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/19 16:38:34 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*set_back_dir(t_env *env)
{
	char	*path;
	char	*tmp;
	int		i;

	path = find_env_value(env, "PWD");
	if (!path)
		return (NULL);
	i = ft_strlen(path) - 1;
	while (i >= 0 && path[i] != '/')
		i--;
	if (i == 0)
		return (ft_strdup("/"));
	tmp = ft_substr(path, 0, i);
	free(path);
	return (tmp);
}

int	replace_path(t_env *env, char *name, char *path)
{
	char	*line;
	char	*tmp;

	line = find_env_line(env, name);
	if (!line)
		return (1);
	tmp = ft_strjoin(name, "=");
	free(line);
	line = ft_strjoin(tmp, path);
	delete_replace(&env, name, line);
	free(tmp);
	free(line);
	return (0);
}

int	cd_command(t_data *data, t_token *token)
{
	char	*path;
	char	*old_pwd;
	t_token	*cd_token;

	old_pwd = find_env_value(data->env, "PWD");
	if (!old_pwd)
		return (1);
	cd_token = token->next;
	if (!cd_token)
		path = find_env_value(data->env, "HOME");
	else if (!ft_strcmp(cd_token->value, "-"))
		path = find_env_value(data->env, "OLDPWD");
	else if (!ft_strcmp(cd_token->value, "."))
		path = find_env_value(data->env, "PWD");
	else if (!ft_strcmp(cd_token->value, ".."))
		path = set_back_dir(data->env);
	else
		path = cd_token->value;
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		printf("cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	replace_path(data->env, "OLDPWD", old_pwd);
	replace_path(data->env, "PWD", getcwd(NULL, 0));
	return (0);
}
