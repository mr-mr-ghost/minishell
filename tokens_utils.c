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

char	*remove_spaces(char *line)
{
	int		i;
	int		j;
	char	*new_line;

	i = 0;
	j = 0;
	new_line = (char *)malloc(sizeof(char) * (ft_strlen(line) + 1));
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
		{
			new_line[j++] = line[i++];
			while (line[i] && line[i] != '\"' && line[i] != '\'')
				new_line[j++] = line[i++];
			if (line[i] == '\"' || line[i] == '\'')
				new_line[j++] = line[i++];
		}
		else if (line[i] != ' ')
			new_line[j++] = line[i++];
		else
			i++;
	}
	new_line[j] = '\0';
	return (new_line);
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

int	is_cmd(char *line, int i)
{
	if (select_cmp(line, "echo", i, 4))
		return (4);
	else if (select_cmp(line, "cd", i, 2))
		return (2);
	return (0);
}
