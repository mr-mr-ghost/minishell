/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/09 15:10:20 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(char **cmd)
{
	int	i;

	if (cmd != NULL)
	{
		i = 0;
		while (cmd[i])
			free(cmd[i++]);
		free(cmd);
	}
}

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

	cmd = NULL;
	cmd = read_and_split(cmd);
	i = 0;
	while (cmd && cmd[i])
		ft_printf("%s\n", cmd[i++]);
	free_cmd(cmd);
	return (0);
}
