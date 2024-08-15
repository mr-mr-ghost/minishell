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

void	token_split(t_data *data)
{
	char	*line;
	bool	echo;
	int		i;

	if (!data->line)
		line = ft_strdup("");
	else
		line = ft_strdup(data->line);
	i = 0;
	echo = false;
	while (line[i])
	{
		if (line[i] == ' ')
			i++;
		else if (is_cmd(line, i))
			echo = handle_cmd(data, line, &i);
		else if ((line[i] == '\"' || line[i] == '\'') && quotes_check(line, i))
			handle_quotes(data, line, &i);
		else if (ft_strchr("><|;", line[i]))
			handle_special_chars(data, line, &i);
		else
			handle_normal_chars(data, line, &i, echo);
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
		else
			tmp->type = ARG;
		tmp = tmp->next;
	}
}
