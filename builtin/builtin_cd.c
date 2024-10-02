/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:38:34 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/02 14:49:37 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*set_back_dir(void)
{
	char	*path;
	char	*tmp;
	int		i;

	path = getcwd(NULL, 0);
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

void	replace_path(t_env *env, char *name, char *path)
{
	char	*line;
	char	*tmp;

	tmp = ft_strjoin(name, "=");
	line = ft_strjoin(tmp, path);
	env_replace(env, name, line);
	free(tmp);
	free(line);
}

int	change_env_path(t_data *data, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (1);
	replace_path(data->env, "OLDPWD", old_pwd);
	replace_path(data->env, "PWD", new_pwd);
	replace_path(data->secret_env, "OLDPWD", old_pwd);
	replace_path(data->secret_env, "PWD", new_pwd);
	free(new_pwd);
	free(old_pwd);
	return (0);
}

char	*determine_path(t_env *env, t_token *token)
{
	char	*path;

	if (!token || token->type >= TRUNC || !ft_strcmp(token->value, "~"))
		path = find_env_value(env, "HOME");
	else if (!ft_strcmp(token->value, "-"))
		path = find_env_value(env, "OLDPWD");
	else if (!token->type || !ft_strcmp(token->value, "."))
		path = getcwd(NULL, 0);
	else if (!ft_strcmp(token->value, ".."))
		path = set_back_dir();
	else
		path = ft_strdup(token->value);
	return (path);
}

int	cd_command(t_data *data, t_token *token)
{
	char	*path;
	char	*old_pwd;
	int		status;
	t_token	*cd_token;

	cd_token = token->next;
	if (cd_token->next && cd_token->next->type <= ARG)
		return (err_msg("cd", NULL, "too many arguments", 1));
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (1);
	path = determine_path(data->env, cd_token);
	status = chdir(path);
	if (status < 0)
	{
		free(old_pwd);
		status = err_msg("cd", path, strerror(errno), 1);
	}
	free(path);
	if (status || change_env_path(data, old_pwd))
		return (1);
	return (0);
}
