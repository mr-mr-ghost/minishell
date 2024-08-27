/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_arr_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:07:19 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/26 14:07:19 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* frees the command array*/
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

/* counts tokens up until given type is met*/
int	count_args(t_token *token, int maxtype)
{
	t_token	*tmp;
	int		count;

	if (!token)
		return (0);
	count = 0;
	tmp = token;
	while (tmp && tmp->type < maxtype)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

/* forms a string array of 1 command*/
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
		i++;
		tmp = tmp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}

/* forms a string array of minishell environment variables*/
char	**form_enva(t_env *env)
{
	char	**array;
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	array = (char **)malloc((i + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	tmp = env;
	while (tmp)
	{
		array[i++] = tmp->line;
		tmp = tmp->next;
	}
	array[i] = NULL;
	return (array);
}
