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
	new = token_new(ft_substr(line, *i, j - *i));
	if (!new || !new->value)
		return (1);
	token_add_back(&data->token, new);
	*i = j;
	return (0);
}

void	handle_quotes(t_data *data, char *buffer, int *i, int *k)
{
	int		j;

	j = *i + 1;
	while (data->line[j] && data->line[j] != data->line[*i])
	{
		if (data->line[*i] == '\"' && data->line[j] == '$' && data->line[j + 1]
			&& data->line[j + 1] != ' ')
			add_dollar_value(data, buffer, &j, k);
		else
			buffer[(*k)++] = data->line[j++];
	}
	if (data->line[j] == data->line[*i])
		j++;
	*i = j;
}

int	handle_cmd(t_data *data, char *line, int *i)
{
	int		j;
	t_token	*new;

	j = *i + is_cmd(line, *i);
	new = token_new(ft_substr(line, *i, j - *i));
	if (!new || !new->value)
		return (1);
	token_add_back(&data->token, new);
	*i = j;
	return (0);
}

int	handle_normal_chars(t_data *data, int *i)
{
	char	buffer[BUFF_SIZE];
	int		j;
	int		k;
	t_token	*new;

	j = *i;
	k = 0;
	while (data->line[j] && !ft_strchr("><|; ", data->line[j]) && k < BUFF_SIZE)
	{
		if (data->line[j] == '\"' || data->line[j] == '\'')
			handle_quotes(data, buffer, &j, &k);
		else if (data->line[j] == '$' && data->line[j + 1]
			&& data->line[j + 1] != ' ')
			add_dollar_value(data, buffer, &j, &k);
		else
			buffer[k++] = data->line[j++];
	}
	buffer[k] = '\0';
	new = token_new(ft_strdup(buffer));
	if (!new || !new->value)
		return (1);
	token_add_back(&data->token, new);
	*i = j;
	return (0);
}
