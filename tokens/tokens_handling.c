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

int	is_cmd(char *line, int i)
{
	if (select_cmp(line, "echo", i, 4)
		&& ft_strchr("><|;\"\' \0", line[i + 4]))
		return (4);
	else if (select_cmp(line, "cd", i, 2)
		&& ft_strchr("><|;\"\' \0", line[i + 2]))
		return (2);
	else if (select_cmp(line, "pwd", i, 3)
		&& ft_strchr("><|;\"\' \0", line[i + 3]))
		return (3);
	else if (select_cmp(line, "export", i, 6)
		&& ft_strchr("><|;\"\' \0", line[i + 6]))
		return (6);
	else if (select_cmp(line, "unset", i, 5)
		&& ft_strchr("><|;\"\' \0", line[i + 5]))
		return (5);
	else if (select_cmp(line, "env", i, 3)
		&& ft_strchr("><|;\"\' \0", line[i + 3]))
		return (3);
	else if (select_cmp(line, "exit", i, 4)
		&& ft_strchr("><|;\"\' \0", line[i + 4]))
		return (4);
	return (0);
}

void	handle_edge_case(t_data *data, char *line, int *i, int edge)
{
	if (edge == 1)
		handle_echo_chars(data, line, i);
	else if (edge == 2)
		handle_export_chars(data, line, i);
}

void	process_token(t_data *data, char *line)
{
	int		edge;
	int		i;

	i = 0;
	edge = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			i++;
		else if (is_cmd(line, i))
			edge = handle_cmd(data, line, &i);
		else if (edge && !ft_strchr("><|;", line[i]))
			handle_edge_case(data, line, &i, edge);
		else if (ft_strchr("><|;", line[i]))
		{
			handle_special_chars(data, line, &i);
			edge = 0;
		}
		else if ((line[i] == '\"' || line[i] == '\'')
			&& select_quotes_check(line, i))
			handle_quotes(data, line, &i);
		else
			handle_normal_chars(data, line, &i);
	}
}

int	token_split(t_data *data)
{
	char	*line;

	add_history(data->line);
	if (!data->line)
		line = ft_strdup("");
	else
		line = ft_strdup(data->line);
	if (!line)
		return (1);
	if (quotes_check(line))
	{
		err_msg(NULL, line, "Unclosed quotes", 1);
		free(line);
		free(data->line);
		return (1);
	}
	process_token(data, line);
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
