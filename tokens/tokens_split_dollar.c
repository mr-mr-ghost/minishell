/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_split_dollar.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:20:14 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/09 14:13:17 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_dollar_value(t_env *env, char *line, int *start)
{
	char	*env_name;
	char	*env_value;

	env_name = find_env_name(env, line + *start);
	if (!env_name)
	{
		*start = update_line_iter(line, *start);
		return (NULL);
	}
	*start += ft_strlen(env_name);
	env_value = find_env_value(env, env_name);
	free(env_name);
	return (env_value);
}

int	update_line_iter(char *line, int i)
{
	if (ft_isdigit(line[i]))
	{
		while (line[i] && ft_isdigit(line[i]))
			i++;
	}
	else if (ft_isalpha(line[i]) || line[i] == '_')
	{
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
	}
	else
		i++;
	return (i);
}

char	*process_dollar(t_data *data, char *line, int *i)
{
	char	*env_value;

	(*i)++;
	if (line[*i] == '?')
	{
		env_value = ft_itoa(data->exit_code);
		(*i)++;
	}
	else if (line[*i] == '$')
	{
		env_value = find_env_value(data->env, "SYSTEMD_EXEC_PID");
		(*i)++;
	}
	else if (select_valid_env(data->env, line, *i))
		env_value = get_dollar_value(data->env, line, i);
	else if (!select_valid_env(data->env, line, *i)
		&& (!ft_strchr("><|%+,./:=^~\" ", line[*i])))
	{
		env_value = ft_strdup("");
		*i = update_line_iter(line, *i);
	}
	else
		env_value = ft_strdup("$");
	return (env_value);
}

bool	add_dollar_value(t_data *data, char *buffer, int *j, int *k)
{
	char	*env_value;
	int		i;
	bool	quote;

	quote = false;
	if (*j > 0 && data->line[*j - 1] && data->line[*j - 1] == '\"')
		quote = true;
	env_value = process_dollar(data, data->line, j);
	i = 0;
	while (env_value && env_value[i] && *k < ARG_MAX - 1)
		buffer[(*k)++] = env_value[i++];
	if (env_value)
		free(env_value);
	if ((*k) == 0 && !quote)
		while (data->line[*j] && data->line[*j] == ' ')
			(*j)++;
	return (true);
}
