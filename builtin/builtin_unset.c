/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 15:30:44 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/19 15:30:44 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	del_env(t_env **env, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, key))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			env_delone(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	unset_env(t_data *data, t_token *token)
{
	char	*env_name;

	if (!token->value)
		return (1);
	if (ft_strchr(token->value, '='))
		return (0);
	env_name = find_env_name(data->env, token->value);
	if (!env_name)
		return (0);
	del_env(&data->env, env_name);
	del_env(&data->secret_env, env_name);
	free(env_name);
	return (0);
}

int	unset_command(t_data *data, t_token *token)
{
	int		status;
	t_token	*unset_token;

	if (!token->next)
		return (0);
	unset_token = token->next;
	status = 0;
	while (unset_token && unset_token->type <= ARG)
	{
		if (unset_token->type == ARG)
			status = unset_env(data, unset_token);
		unset_token = unset_token->next;
	}
	return (status);
}
