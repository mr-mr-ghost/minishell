/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:05:12 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/13 14:05:12 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*token_new(char *value)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	new->value = ft_strdup(value);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	token_add_back(t_token **token, t_token *new)
{
	t_token	*tmp;

	if (*token == NULL)
	{
		*token = new;
		return ;
	}
	tmp = *token;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	new->prev = tmp;
}
