/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 00:41:44 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/18 11:07:22 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// frees the command array
void	free_cmd(char **cmd)
{
	if (!cmd)
		return ;
	free(cmd);
	cmd = NULL;
}

// counts tokens up until given type is met
int	count_args(t_token *token, int type)
{
	t_token	*tmp;
	int		count;

	if (!token)
		return (0);
	count = 0;
	tmp = token;
	while (tmp && tmp->type < type)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

// forms a string array of 1 command
char	**form_cmd(t_token *token, int len)
{
	char	**cmd;
	t_token	*tmp;
	int		i;

	cmd = (char **)malloc((len + 1) * sizeof(char *));
	if (!cmd)
		return (NULL);
	i = 0;
	tmp = token;
	while (tmp && i < len)
	{
		cmd[i] = tmp->value;
		/* if (!cmd[i])
		{
			free_cmd(cmd);
			return (NULL);
		} */
		i++;
		tmp = tmp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}
