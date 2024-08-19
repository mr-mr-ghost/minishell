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

int	handle_cmd(t_data *data, char *line, int *i)
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
	if (!ft_strncmp(line, "echo", 4))
		return (1);
	else if (!ft_strncmp(line, "export", 6))
		return (2);
	return (0);
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

void	handle_echo_chars(t_data *data, char *line, int *i)
{
	int		j;
	char	*new;

	j = *i;
	if (line[j] == '-' && select_cmp(line, "-n ", j, 3))
	{
		token_add_back(&data->token, token_new(ft_substr(line, j, 2)));
		j += 2;
		while (line[j] && line[j] == ' ')
			j++;
		*i = j;
	}
	while (line[j] && !ft_strchr("><|;", line[j]))
		j++;
	if (j == *i)
		return ;
	new = token_remove_quotes(ft_substr(line, *i, j - *i));
	token_add_back(&data->token, token_new(new));
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

void	handle_export_chars(t_data *data, char *line, int *i)
{
	int		j;

	j = *i + 1;
	while (line[j] && !ft_strchr("><|; ", line[j]))
		j++;
	token_add_back(&data->token, token_new(ft_substr(line, *i, j - *i)));
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
}
