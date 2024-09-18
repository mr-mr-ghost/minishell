/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_split_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:21:30 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/13 14:21:30 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_special_chars(t_data *data, char *line, int *i)
{
	int		j;
	t_token	*new;

	j = *i + 1;
	if ((line[*i] == '<' && line[*i + 1] == '<')
		|| (line[*i] == '>' && line[*i + 1] == '>'))
		j++;
	new = token_new(ft_substr(line, *i, j - *i), false);
	if (!new || !new->value)
		return (1);
	token_add_back(&data->token, new);
	*i = j;
	return (0);
}

bool	handle_quotes(t_data *data, char *buffer, int *i, int *k)
{
	int		j;
	bool	div;

	j = *i + 1;
	div = false;
	while (data->line[j] && data->line[j] != data->line[*i])
	{
		if (data->line[*i] == '\"' && data->line[j] == '$' && data->line[j + 1]
			&& data->line[j + 1] != ' ')
			add_dollar_value(data, buffer, &j, k);
		else if (ft_strchr("><|;", data->line[j]))
		{
			div = true;
			buffer[(*k)++] = data->line[j++];
			if ((data->line[j - 1] == '>' && data->line[j] == '>')
				|| (data->line[j - 1] == '<' && data->line[j] == '<'))
				buffer[(*k)++] = data->line[j++];
		}
		else
			buffer[(*k)++] = data->line[j++];
	}
	if (data->line[j] == data->line[*i])
		j++;
	*i = j;
	return (div);
}

int	handle_cmd(t_data *data, char *line, int *i)
{
	int		j;
	t_token	*new;

	j = *i + is_cmd(line, *i);
	new = token_new(ft_substr(line, *i, j - *i), false);
	if (!new || !new->value)
		return (1);
	token_add_back(&data->token, new);
	*i = j;
	return (0);
}

int	handle_normal_chars(t_data *data, int *i)
{
	char	buffer[BUFF_SIZE];
	int		k;
	bool	div;
	t_token	*new;

	k = 0;
	div = false;
	while (data->line[*i] && !ft_strchr("><|; ", data->line[*i])
		&& k < BUFF_SIZE)
	{
		if (data->line[*i] == '\"' || data->line[*i] == '\'')
			div = handle_quotes(data, buffer, i, &k);
		else if (data->line[*i] == '$' && data->line[(*i) + 1]
			&& data->line[(*i) + 1] != ' ')
			add_dollar_value(data, buffer, i, &k);
		else
			buffer[k++] = data->line[(*i)++];
	}
	buffer[k] = '\0';
	new = token_new(ft_strdup(buffer), div);
	if (!new || !new->value)
		return (1);
	token_add_back(&data->token, new);
	return (0);
}
