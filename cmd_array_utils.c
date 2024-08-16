/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 00:41:44 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/17 01:08:26 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// counts all the tokens for 1 command
int	count_args(t_token *token)
{
	t_token	*tmp;
	int		len;

	if (!token)
		return (0);
	len = 1;
	tmp = token->next;
	while (tmp && tmp->type < TRUNC)
	{
		len++;
		tmp = tmp->next;
	}
	return (len);
}

// forms a string array of 1 command
char	**form_cmd(t_token *token, int len)
{
	char	**cmd;
	t_token	*tmp;
	int		i;

	if (len == 0)
		return (NULL);
	cmd = (char **)malloc((len + 1) * sizeof(char *));
	if (!cmd)
		return (NULL);
	i = 0;
	tmp = token;
	while (tmp && i < len)
	{
		cmd[i] = ft_strdup(tmp->value);
		if (!cmd[i])
		{
			free_cmd(cmd);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

// delete 1 command from the token list
/* void	delete_command_from_list(t_token **token, int clen)
{

} */
