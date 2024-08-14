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

void	handle_cmd(t_data *data, char *line, int *i)
{
	int	j;

	j = *i;
	j += is_cmd(line, j);
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

	if (!data->line)
		line = ft_strdup("");
	else
		line = remove_spaces(data->line);
	i = 0;
	while (line[i])
	{
		if (is_cmd(line, i))
			handle_cmd(data, line, &i);
		else if (ft_strchr("><|;", line[i]))
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
		if (!tmp->value || !ft_strcmp(tmp->value, ""))
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
