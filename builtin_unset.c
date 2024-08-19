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

#include "minishell.h"

int	del_env(t_env **env, char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, key, ft_strlen(tmp->name)))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			env_delone(tmp);
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

int	unset_command(t_data *data, t_token *token)
{
	int	status;

	if (!token->next)
		return (1);
	status = del_env(&data->env, token->next->value);
	del_env(&data->secret_env, token->next->value);
	return (status);
}
