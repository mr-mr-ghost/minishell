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
	if (!new)
		return (NULL);
	if (!value)
		new->value = NULL;
	else
		new->value = value;
	new->type = 0;
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

bool	select_cmp(char *line, char *cmp, int start, int len)
{
	int	i;

	i = 0;
	while (line[start] && cmp[i] && line[start] == cmp[i] && i < len)
	{
		start++;
		i++;
	}
	if (i == len)
		return (true);
	return (false);
}

bool	quotes_check(char *line, int i)
{
	int	j;

	j = i + 1;
	while (line[j] && (line[j] != line[i]))
		j++;
	if (line[j] == line[i])
		return (true);
	return (false);
}

int	is_cmd(char *line, int i)
{
	if (select_cmp(line, "echo -n ", i, 8))
		return (7);
	else if (select_cmp(line, "echo ", i, 5))
		return (4);
	else if (select_cmp(line, "cd ", i, 3))
		return (2);
	else if (select_cmp(line, "pwd ", i, 4))
		return (2);
	else if (select_cmp(line, "export ", i, 7))
		return (6);
	else if (select_cmp(line, "unset ", i, 6))
		return (5);
	else if (select_cmp(line, "env ", i, 4))
		return (3);
	else if (select_cmp(line, "exit ", i, 5))
		return (4);
	return (0);
}
