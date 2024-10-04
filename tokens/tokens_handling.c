/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:05:48 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/03 13:44:51 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	tokens_type_define(t_data *data)
{
	t_token	*tmp;

	tmp = data->token;
	while (tmp)
	{
		if (!tmp->value || !ft_strcmp(tmp->value, ""))
			tmp->type = EMPTY;
		else if (tmp->div && !ft_strcmp(tmp->value, ">"))
			tmp->type = TRUNC;
		else if (tmp->div && !ft_strcmp(tmp->value, ">>"))
			tmp->type = APPEND;
		else if (tmp->div && !ft_strcmp(tmp->value, "<<"))
			tmp->type = HEREDOC;
		else if (tmp->div && !ft_strcmp(tmp->value, "<"))
			tmp->type = INPUT;
		else if (tmp->div && !ft_strcmp(tmp->value, "|"))
			tmp->type = PIPE;
		else if (!tmp->prev || tmp->prev->type >= TRUNC)
			tmp->type = CMD;
		else
			tmp->type = ARG;
		tmp = tmp->next;
	}
}

int	process_token(t_data *data)
{
	int	ret;
	int	i;

	i = 0;
	ret = 0;
	while (data->line[i])
	{
		if (data->line[i] == ' ')
			i++;
		else if (ft_strchr("><|", data->line[i]))
			ret = handle_special_chars(data, data->line, &i);
		else
			ret = handle_normal_chars(data, &i);
		if (ret)
			return (ret);
	}
	return (ret);
}

int	parse_tokens(t_data *data, t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == PIPE && !tmp->prev)
			return (syntax_err(data, "|"));
		else if ((tmp->type >= TRUNC && tmp->type <= HEREDOC)
			&& tmp->next && tmp->next->type >= TRUNC)
			return (syntax_err(data, tmp->next->value));
		else if ((tmp->type >= TRUNC && tmp->type <= HEREDOC) && !tmp->next)
			return (syntax_err(data, "newline"));
		else if (tmp->type == PIPE && !tmp->next)
			return (token_err(data, NULL,
					"syntax error: unexpected end of file", 2));
		tmp = tmp->next;
	}
	return (0);
}

int	token_split(t_data *data)
{
	if (data->line[0])
		add_history(data->line);
	if (ft_strlen(data->line) >= ARG_MAX)
		return (token_err(data, NULL, "Argument list too long", 126));
	if (quotes_check(data->line))
		return (token_err(data, data->line,
				"syntax error: unclosed quotes", 2));
	if (process_token(data))
		return (token_err(data, NULL, "memory allocation error", 1));
	tokens_type_define(data);
	return (parse_tokens(data, data->token));
}
