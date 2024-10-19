/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 13:50:30 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/19 21:51:54 by gklimasa         ###   ########.fr       */
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
	char	*string;
	char	buffer[ARG_MAX];

	i = 0;
	j = 0;
	while (s2 && s2[j] && i < ARG_MAX - 1)
	{
		if (s2[j] == '$' && s2[j + 1] && s2[j + 1] != ' ' && s2[j + 1] != '\n')
		{
			string = process_dollar(data, s2, &j);
			i = heredoc_strcat(buffer, string, i);
		}
		else
			buffer[i++] = s2[j++];
	}
	buffer[i] = '\0';
	if (s2)
		free(s2);
	string = ft_strjoin(s1, buffer);
	if (s1)
		free(s1);
	return (string);
}

char	*heredoc_error(char *delimiter, char *heredoc)
{
	if (g_sigint)
	{
		if (heredoc)
			free(heredoc);
		return (NULL);
	}
	ft_putchar_fd('\n', 1);
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
	return (heredoc);
}

// check for heredoc token, retrieve heredoc char, open heredoc pipe
// on success return heredoc file
// on failure return NULL
char	*set_heredoc(t_data *data, t_token *currentt, t_pvars *pvars)
{
	char	*heredoc;

	pvars->htoken = return_1stheredoct(currentt);
	if (pvars->htoken)
	{
		heredoc = get_heredoc(data, pvars->htoken->next->value);
		if (g_sigint)
			return (NULL);
		if (!is_pipe(heredoc, pvars->pfd[2], &(pvars->status)))
		{
			if (heredoc)
				free(heredoc);
			pvars->htoken = NULL;
			return (NULL);
		}
		return (heredoc);
	}
	return (NULL);
}

// send heredoc through heredoc pipe, free it and set it to NULL
void	send_clean_heredoc(t_pvars *pvars)
{
	if (pvars->htoken)
	{
		ft_putstr_fd(pvars->hdoc, pvars->pfd[2][1]);
		edit_pipeset(pvars->pfd[2], NULL, 0, 1);
		if (pvars->hdoc)
		{
			free(pvars->hdoc);
			pvars->hdoc = NULL;
		}
	}
	pvars->htoken = NULL;
}
