/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_split_special.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 12:20:14 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/27 12:20:14 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_export_chars(t_data *data, char *line, int *i)
{
	int		j;
	t_token	*new;

	j = *i + 1;
	while (line[j] && !ft_strchr("><|; ", line[j]))
		j++;
	new = token_new(ft_substr(line, *i, j - *i));
	if (!new || !new->value)
		return (1);
	token_add_back(&data->token, new);
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
	return (0);
}

void	handle_echo_quotes(char *line, int *i)
{
	int		j;

	j = *i + 1;
	while (line[j] && line[j] != line[*i])
		j++;
	if (line[j] == line[*i])
		j++;
	*i = j;
}

int	handle_echo_option(t_data *data, char *line, int *i)
{
	int		j;
	t_token	*new;

	j = *i;
	if (line[j] == '-' && select_cmp(line, "-n ", j, 3))
	{
		new = token_new(ft_substr(line, j, 2));
		if (!new || !new->value)
			return (1);
		token_add_back(&data->token, new);
		j += 2;
		while (line[j] && line[j] == ' ')
			j++;
		*i = j;
	}
	return (0);
}

int	handle_echo_chars(t_data *data, char *line, int *i)
{
	int		j;
	t_token	*new;

	j = *i;
	if (handle_echo_option(data, line, i))
		return (1);
	while (line[j] && !ft_strchr("><|;", line[j]))
	{
		if (line[j] == '\"' || line[j] == '\'')
			handle_echo_quotes(line, &j);
		else
			j++;
	}
	while (line[j - 1] && line[j - 1] == ' ')
		j--;
	if (j == *i)
		return (0);
	new = token_new(ft_substr(line, *i, j - *i));
	if (!new || !new->value)
		return (1);
	token_add_back(&data->token, new);
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
	return (0);
}

int	handle_cmd(t_data *data, char *line, int *i)
{
	int		j;
	t_token	*new;

	j = *i + is_cmd(line, *i);
	new = token_new(ft_substr(line, *i, j - *i));
	if (!new || !new->value)
		return (3);
	if (*i == 0)
		data->token = new;
	else
		token_add_back(&data->token, new);
	while (line[j] && line[j] == ' ')
		j++;
	*i = j;
	if (!ft_strncmp(line, "echo", 4))
		return (1);
	else if (!ft_strncmp(line, "export", 6))
		return (2);
	return (0);
}
