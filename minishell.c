/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/12 08:48:58 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**read_and_split(char **cmd)
{
	char	*line;

	line = readline("Minishell> ");
	cmd = ft_split(line, ' ');
	if (cmd == NULL || cmd[0] == NULL)
		return (NULL);
	free(line);
	return (cmd);
}

int	main(void)
{
	char	**cmd;
	int		i;

	while (1)
	{
		cmd = NULL;
		cmd = read_and_split(cmd);
		i = 0;
		while (cmd && cmd[i])
			ft_printf("%s\n", cmd[i++]);
		free_cmd(cmd);
	}
	return (0);
}
