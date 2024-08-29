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

#include "../minishell.h"

/*int	is_cmd(char *line, int i)
{
	const char	*cmds[] = {"echo", "cd", "pwd",
		"export", "unset", "env", "exit"};
	const int	lengths[] = {4, 2, 3, 6, 5, 3, 4};
	const int	num_cmds = sizeof(cmds) / sizeof(cmds[0]);
	int			j;

	j = 0;
	while (j < num_cmds)
	{
		if (select_cmp(line, cmds[j], i, lengths[j])
			&& ft_strchr("><|;\"\' \0", line[i + lengths[j]]))
			return (lengths[j]);
		j++;
	}
	return (0);
}*/

int	handle_edge_case(t_data *data, char *line, int *i, int edge)
{
	if (edge == 1)
		return (handle_echo_chars(data, line, i));
	else if (edge == 2)
		handle_export_chars(data, line, i);
	return (0);
}

int	process_char(t_data *data, char *line, int *i, int *edge)
{
	int	ret;

	ret = 0;
	if (line[*i] == ' ')
		(*i)++;
	else if (is_cmd(line, *i))
		*edge = handle_cmd(data, line, i);
	else if ((*edge && *edge != 3) && !ft_strchr("><|;", line[*i]))
		ret = handle_edge_case(data, line, i, *edge);
	else if (ft_strchr("><|;", line[*i]))
	{
		ret = handle_special_chars(data, line, i);
		*edge = 0;
	}
	else if ((line[*i] == '\"' || line[*i] == '\'')
		&& select_quotes_check(line, *i))
		ret = handle_quotes(data, line, i);
	else
		ret = handle_normal_chars(data, line, i);
	return (ret);
}

int	process_token(t_data *data, char *line)
{
	int	edge;
	int	ret;
	int	i;

	edge = 0;
	i = 0;
	while (line[i])
	{
		ret = process_char(data, line, &i, &edge);
		if (ret || edge == 3)
			return (1);
	}
	return (0);
}

int	token_split(t_data *data)
{
	char	*line;

	if (data->line[0] != '\0')
		add_history(data->line);
	if (!data->line)
		line = ft_strdup("");
	else
		line = ft_strdup(data->line);
	if (!line)
		return (1);
	if (quotes_check(line))
		return (token_err(data, line, "Unclosed quotes", 1));
	if (process_token(data, line))
	{
		free(line);
		return (token_err(data, NULL, "Memory allocation failure", 1));
	}
	free(line);
	tokens_type_define(data);
	return (0);
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
		else if (!ft_strcmp(tmp->value, "<<"))
			tmp->type = HEREDOC;
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
