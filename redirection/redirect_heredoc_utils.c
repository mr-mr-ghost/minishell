/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:50:30 by jhoddy            #+#    #+#             */
/*   Updated: 2024/09/25 13:50:38 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	heredoc_strcat(char *dest, char *src, int start)
{
	int	j;

	j = 0;
	while (src && src[j])
		dest[start++] = src[j++];
	if (src)
		free(src);
	return (start);
}

char	*join_strings(t_data *data, char *s1, char *s2)
{
	int		i;
	int		j;
	char	*env_value;
	char	buffer[BUFF_SIZE];

	i = heredoc_strcat(buffer, s1, 0);
	j = 0;
	while (s2 && s2[j] && i < BUFF_SIZE)
	{
		if (s2[j] == '$' && s2[j + 1] && s2[j + 1] != ' ' && s2[j + 1] != '\n')
		{
			env_value = process_dollar(data, s2, &j);
			i = heredoc_strcat(buffer, env_value, i);
		}
		else
			buffer[i++] = s2[j++];
	}
	buffer[i] = '\0';
	if (s2)
		free(s2);
	return (ft_strdup(buffer));
}

char	*heredoc_error(char *delimiter, char *heredoc)
{
	ft_putchar_fd('\n', 1);
	if (g_sig.sigint)
	{
		if (heredoc)
			free(heredoc);
		return (NULL);
	}
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
	return (heredoc);
}
