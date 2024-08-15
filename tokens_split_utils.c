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

bool	handle_cmd(t_data *data, char *line, int *i)
{
	int	j;

	j = *i + is_cmd(line, *i);
	if (*i == 0)
		data->token = token_new(ft_substr(line, *i, j - *i));
	else
		token_add_back(&data->token, token_new(ft_substr(line, *i, j - *i)));
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
	return (!ft_strncmp(line, "echo", 4));
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

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	new = (char *)malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
			new[j++] = str[i];
		i++;
	}
	new[j] = '\0';
	free(str);
	return (new);
}

void	handle_normal_chars(t_data *data, char *line, int *i, bool *check)
{
	int		j;
	char	*new;

	j = *i + 1;
	if (check)
		while (line[j] && !ft_strchr("><|;", line[j]))
			j++;
	else
		while (line[j] && !ft_strchr("><|;\"\' ", line[j]))
			j++;
	new = ft_substr(line, *i, j - *i);
	if (check)
		new = remove_quotes(new);
	if (*i == 0)
		data->token = token_new(new);
	else
		token_add_back(&data->token, token_new(new));
	while (line[j] && line[j] == ' ')
		j++;
	check = false;
	*i = j;
}
