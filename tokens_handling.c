/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:05:48 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/16 19:48:42 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_validc(char c)
{
	if (c == ' ' || c == '\0' || c == '\"'
		|| c == '>' || c == '<' || c == '|' || c == ';')
		return (1);
	return (0);
}

int	is_cmd(char *line, int i)
{
	if (ft_memcmp(line + i, "echo", 4) == 0 && is_validc(line[i + 4]))
		return (4);
	else if (ft_memcmp(line + i, "cd", 2) == 0 && is_validc(line[i + 2]))
		return (2);
	else if (ft_memcmp(line + i, "pwd", 3) == 0 && is_validc(line[i + 3]))
		return (3);
	else if (ft_memcmp(line + i, "export", 6) == 0 && is_validc(line[i + 6]))
		return (6);
	else if (ft_memcmp(line + i, "unset", 5) == 0 && is_validc(line[i + 5]))
		return (5);
	else if (ft_memcmp(line + i, "env", 3) == 0 && is_validc(line[i + 3]))
		return (3);
	else if (ft_memcmp(line + i, "exit", 4) == 0 && is_validc(line[i + 4]))
		return (4);
	return (0);
}

void	process_token(t_data *data, char *line)
{
	bool	echo;
	int		i;

	i = 0;
	echo = false;
	while (line[i])
	{
		if (line[i] == ' ')
			i++;
		else if (is_cmd(line, i))
			echo = handle_cmd(data, line, &i);
		else if (echo)
			echo = handle_echo_chars(data, line, &i);
		else if ((line[i] == '\"' || line[i] == '\'') && quotes_check(line, i))
			handle_quotes(data, line, &i);
		else if (ft_strchr("><|;", line[i]))
			handle_special_chars(data, line, &i);
		else
			handle_normal_chars(data, line, &i);
	}
}

void	token_split(t_data *data)
{
	char	*line;

	if (!data->line)
		line = ft_strdup("");
	else
		line = ft_strdup(data->line);
	process_token(data, line);
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
		else if (!ft_strcmp(tmp->value, "<<"))
			tmp->type = HEREDOC;
		else if (!tmp->prev || tmp->prev->type >= TRUNC)
		{
			printf("checking value %s\n", tmp->value);
			if (is_cmd(tmp->value, 0))
				tmp->type = BCMD;
			else
				tmp->type = NCMD;
		}
		else
			tmp->type = ARG;
		tmp = tmp->next;
	}
}
