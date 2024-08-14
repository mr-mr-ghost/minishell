/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:05:48 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/13 14:05:48 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_flags(t_data *data, char *line, int *i)
{
	int	j;

	j = *i + 1;
	while (line[j] && !ft_strchr("><|;\'\"", line[j]))
		j++;
	if (*i == 0)
		data->token = token_new(ft_substr(line, *i, j - *i));
	else
		token_add_back(&data->token, token_new(ft_substr(line, *i, j - *i)));
	*i = j;
}

void	token_split(t_data *data)
{
	char	*line;
	int		i;

	line = remove_spaces(data->line);
	i = 0;
	while (line[i])
	{
		if (ft_strchr("><|;", line[i]))
			handle_special_chars(data, line, &i);
		else if (line[i] == '\"' || line[i] == '\'')
			handle_quotes(data, line, &i);
		else if (line[i] == '-')
			handle_flags(data, line, &i);
		else
			handle_normal_chars(data, line, &i);
	}
	free(line);
	tokens_type_define(data);
}

void	tokens_type_define(t_data *data)
{
	t_token	*tmp;

	tmp = data->token;
	while (tmp)
	{
		if (!ft_strcmp(tmp->value, ""))
			tmp->type = EMPTY;
		else if (!ft_strcmp(tmp->value, ";"))
			tmp->type = END;
		else if (!ft_strcmp(tmp->value, ">"))
			tmp->type = TRUNC;
		else if (!ft_strcmp(tmp->value, ">>"))
			tmp->type = APPEND;
		else if (!ft_strcmp(tmp->value, "<"))
			tmp->type = INPUT;
		else if (!ft_strcmp(tmp->value, "|"))
			tmp->type = PIPE;
		else if (!tmp->prev || tmp->prev->type >= TRUNC)
			tmp->type = CMD;
		else if (!ft_strncmp(tmp->value, "-", 1) && tmp->prev->type == CMD)
			tmp->type = FLAG;
		else
			tmp->type = ARG;
		tmp = tmp->next;
	}
}
