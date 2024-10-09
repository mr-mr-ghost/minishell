/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_split_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 14:21:30 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/09 14:05:27 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_special_chars(t_data *data, char *line, int *i)
{
	int		j;

	j = *i + 1;
	if ((line[*i] == '<' && line[*i + 1] == '<')
		|| (line[*i] == '>' && line[*i + 1] == '>'))
		j++;
	if (token_lst_add(&data->token, ft_substr(line, *i, j - *i), true))
		return (1);
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

int	handle_normal_chars(t_data *data, int *i)
{
	char	buffer[ARG_MAX];
	int		k;
	bool	var;

	k = 0;
	var = false;
	while (data->line[*i] && !ft_strchr("><| ", data->line[*i])
		&& k < ARG_MAX - 1)
	{
		if (data->line[*i] == '\"' || data->line[*i] == '\'')
			handle_quotes(data, buffer, i, &k);
		else if (data->line[*i] == '$' && data->line[(*i) + 1]
			&& data->line[(*i) + 1] != ' ')
			var = add_dollar_value(data, buffer, i, &k);
		else
			buffer[k++] = data->line[(*i)++];
	}
	buffer[k] = '\0';
	if (!buffer[0] && var)
		return (0);
	if (token_lst_add(&data->token, ft_strdup(buffer), false))
		return (1);
	return (0);
}
