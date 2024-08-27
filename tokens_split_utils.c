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

#include "minishell.h"

void	handle_special_chars(t_data *data, char *line, int *i)
{
	int	j;

	j = *i + 1;
	if (line[*i + 1] == '<' || line[*i + 1] == '>')
		j++;
	if (*i == 0)
		data->token = token_new(ft_substr(line, *i, j - *i));
	else
		token_add_back(&data->token, token_new(ft_substr(line, *i, j - *i)));
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
}

void	handle_quotes(t_data *data, char *line, int *i)
{
	int	j;

	j = *i + 1;
	while (line[j] && (line[j] != line[*i]))
		j++;
	if (line[j] == line[*i])
		j++;
	if (*i == 0)
		data->token = token_new(ft_substr(line, *i, j - *i));
	else
		token_add_back(&data->token, token_new(ft_substr(line, *i, j - *i)));
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
}

void	handle_normal_chars(t_data *data, char *line, int *i)
{
	int		j;
	char	*new;

	j = *i + 1;
	while (line[j] && !ft_strchr("><|;\"\' ", line[j]))
		j++;
	if (j == *i)
		return ;
	if (line[j] == '"')
	{
		handle_export_chars(data, line, i);
		return ;
	}
	new = ft_substr(line, *i, j - *i);
	if (*i == 0)
		data->token = token_new(new);
	else
		token_add_back(&data->token, token_new(new));
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
}
