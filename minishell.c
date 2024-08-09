/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/07/11 14:49:01 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_and_split(char **cmd)
{
	char	*line;

	line = readline("Minishell> ");
	cmd = ft_split(line, ' ');
	free(line);
	return (cmd);
}

int main(void)
{
	char	**cmd;

	read_and_split(cmd);
	for (int i = 0; cmd[i]; i++)
		free(cmd[i]);
	free(cmd);
	return (0);
}
