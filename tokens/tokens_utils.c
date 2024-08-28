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

int	token_err(t_data *data, char *arg, char *msg, int code)
{
	if (arg)
	{
		printf("minishell: '%s': %s\n", arg, msg);
		free(arg);
		free(data->line);
	}
	else
	{
		printf("minishell: %s\n", msg);
		free_tokens(data);
	}
	data->exit_code = code;
	return (code);
}
