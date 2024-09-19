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

bool	select_cmp(const char *line, const char *cmp, int start, int len)
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

int	token_err(t_data *data, char *arg, char *msg, int code)
{
	if (arg)
	{
		err_msg(NULL, arg, msg, code);
		free(arg);
	}
	else
	{
		err_msg(NULL, NULL, msg, code);
		free_tokens(data);
	}
	data->exit_code = code;
	return (code);
}
