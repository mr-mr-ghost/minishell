/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_list_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 20:43:03 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/19 20:58:10 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*token_new(char *value, bool div)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	if (!value)
		new->value = NULL;
	else
		new->value = value;
	new->type = 0;
	new->div = div;
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

int	token_lst_add(t_token **token, char *line, bool div)
{
	t_token	*new;

	new = token_new(line, div);
	if (!new || !new->value)
		return (1);
	token_add_back(token, new);
	return (0);
}

// deletes a token and returns the next token or NULL in its place
t_token	*delete_token(t_token *currentt)
{
	t_token	*prevt;
	t_token	*nextt;

	if (!currentt)
		return (NULL);
	prevt = currentt->prev;
	nextt = currentt->next;
	if (prevt)
		prevt->next = nextt;
	if (nextt)
		nextt->prev = prevt;
	free(currentt->value);
	free(currentt);
	return (nextt);
}
