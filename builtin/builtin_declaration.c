/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_declaration.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:21:38 by jhoddy            #+#    #+#             */
/*   Updated: 2024/09/19 17:21:38 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*check_declaration(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp && tmp->type <= ARG)
	{
		if (!ft_strchr(tmp->value, '='))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	handle_declaration(t_env *secret_env, t_token *token)
{
	t_token	*tmp;
	char	*name;
	char	*arg;

	arg = check_declaration(token);
	if (arg)
		return (err_msg(NULL, arg, "Command not found", 127));
	tmp = token;
	while (tmp && tmp->type <= ARG)
	{
		if (valid_env_name(secret_env, tmp->value))
		{
			name = find_env_name(secret_env, tmp->value);
			if (!name)
				return (1);
			env_replace(secret_env, name, tmp->value);
			free(name);
		}
		else if (check_char(tmp->value))
			env_add_back(&secret_env, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
