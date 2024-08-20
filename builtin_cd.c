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
	{
		path = find_env_value(data->env, "HOME");
		if (!path)
			return (1);
	}
	else if (!ft_strcmp(cd_token->value, "-"))
		path = find_env_value(data->env, "OLDPWD");
	else if (!ft_strcmp(cd_token->value, "."))
		path = find_env_value(data->env, "PWD");
	else if (!ft_strcmp(cd_token->value, ".."))
		path = set_back_dir(data->env);
	else
		path = cd_token->value;
	if (chdir(path) == -1)
	{
		printf("cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	delete_replace(&data->env, "OLDPWD", old_pwd);
	delete_replace(&data->env, "PWD", path);
	return (0);
}
