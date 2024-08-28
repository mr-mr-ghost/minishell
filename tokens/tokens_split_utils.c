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
	if (*i == 0)
		data->token = new;
	else
		token_add_back(&data->token, new);
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
	return (0);
}

int	handle_quotes(t_data *data, char *line, int *i)
{
	int		j;
	t_token	*new;

	j = *i + 1;
	while (line[j] && (line[j] != line[*i]))
		j++;
	if (line[j] == line[*i])
		j++;
	new = token_new(ft_substr(line, *i, j - *i));
	if (!new || !new->value)
		return (1);
	if (*i == 0)
		data->token = new;
	else
		token_add_back(&data->token, new);
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
	return (0);
}

int	handle_normal_chars(t_data *data, char *line, int *i)
{
	int		j;
	t_token	*new;

	j = *i + 1;
	while (line[j] && !ft_strchr("><|;\"\' ", line[j]))
		j++;
	if (j == *i)
		return (0);
	if (line[j] == '\"')
	{
		handle_export_chars(data, line, i);
		return (0);
	}
	new = token_new(ft_substr(line, *i, j - *i));
	if (!new || !new->value)
		return (1);
	if (*i == 0)
		data->token = new;
	else
		token_add_back(&data->token, new);
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
	return (0);
}
