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

#include "../minishell.h"

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
	while (i < len)
	{
		if (!line[start] || line[start + i] != cmp[i])
			return (false);
		i++;
	}
	return (true);
}

bool	quotes_check(char *line)
{
	int		i;
	char	tmp;
	bool	quote;

	i = 0;
	quote = false;
	tmp = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (quote && line[i] == tmp)
				quote = false;
			else if (!quote)
			{
				quote = true;
				tmp = line[i];
			}
		}
		i++;
	}
	return (quote);
}

bool	select_quotes_check(char *line, int i)
{
	int	j;

	j = i + 1;
	while (line[j] && (line[j] != line[i]))
		j++;
	return (line[j] == line[i]);
}
