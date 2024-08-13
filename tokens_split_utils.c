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

char	*remove_spaces(char *line)
{
	int		i;
	int		j;
	char	*new_line;

	i = 0;
	j = 0;
	new_line = (char *)malloc(sizeof(char) * (ft_strlen(line) + 1));
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
		{
			new_line[j++] = line[i++];
			while (line[i] && line[i] != '\"' && line[i] != '\'')
				new_line[j++] = line[i++];
			if (line[i] == '\"' || line[i] == '\'')
				new_line[j++] = line[i++];
		}
		else if (line[i] != ' ')
			new_line[j++] = line[i++];
		else
			i++;
	}
	new_line[j] = '\0';
	return (new_line);
}

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
	*i = j;
}

void	handle_normal_chars(t_data *data, char *line, int *i)
{
	int	j;

	j = *i + 1;
	while (line[j] && !ft_strchr("><|\"\'", line[j]))
		j++;
	if (*i == 0)
		data->token = token_new(ft_substr(line, *i, j - *i));
	else
		token_add_back(&data->token, token_new(ft_substr(line, *i, j - *i)));
	*i = j;
}
