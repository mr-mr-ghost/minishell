/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 16:38:58 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/09 16:38:58 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_data *data)
{
	t_token	*tmp;

	if (data->line)
		free(data->line);
	while (data->token)
	{
		tmp = data->token;
		data->token = data->token->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->value)
			free(tmp->value);
		if (tmp->name)
			free(tmp->name);
		if (tmp->line)
			free(tmp->line);
		free(tmp);
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (1);
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*remove_quotes(char *line)
{
	int		i;
	int		j;
	char	*new_line;

	i = 0;
	j = 0;
	new_line = (char *)malloc(sizeof(char) * (ft_strlen(line) + 1));
	if (!new_line)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
			i++;
		else
			new_line[j++] = line[i++];
	}
	new_line[j] = '\0';
	return (new_line);
}

char *add_quotes_var(char *line)
{
	char	*new_line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_line = (char *)malloc(sizeof(char) * (ft_strlen(line) + 3));
	if (!new_line)
		return (NULL);
	while (line[i] && line[i] != '=')
		new_line[j++] = line[i++];
	new_line[j++] = line[i++];
	if (line[i] != '\"')
		new_line[j++] = '\"';
	while (line[i])
		new_line[j++] = line[i++];
	if (line[i] != '\"' && line[i - 1] != '\"')
		new_line[j++] = '\"';
	new_line[j] = '\0';
	return (new_line);
}

char	*ft_strstr(const char *big, const char *little)
{
	size_t	i;
	size_t	j;

	if (!*little)
		return ((char *)big);
	i = 0;
	while (big[i])
	{
		j = 0;
		while (big[i + j] == little[j] && little[j])
			j++;
		if (!little[j])
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}

bool	check_char(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalpha(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int err_msg(char *location, char *msg, int code)
{
	printf("minishell: %s: %s\n", location, msg);
	return (code);
}
