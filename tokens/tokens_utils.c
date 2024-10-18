/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:05:12 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/11 15:00:59 by jhoddy           ###   ########.fr       */
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

bool	select_valid_env(t_env *env, char *line, int start)
{
	char	*env_name;

	env_name = find_env_name(env, line + start);
	if (!env_name)
		return (false);
	if (valid_env_name(env, env_name))
	{
		free(env_name);
		return (true);
	}
	free(env_name);
	return (false);
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
		err_msg(NULL, arg, msg, code);
	else
		err_msg(NULL, NULL, msg, code);
	free_tokens(data);
	data->exit_code = code;
	return (code);
}

int	syntax_err(t_data *data, char *arg)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("'\n", 2);
	free_tokens(data);
	data->exit_code = 2;
	return (2);
}
