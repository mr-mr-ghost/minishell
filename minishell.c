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

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		i;

	(void)argc;
	(void)argv;
	data.cmd = (t_cmd *)malloc(sizeof(t_cmd));
	while (1)
	{
		data.cmd->line = read_and_split(data.cmd->line);
		i = 0;
		while (data.cmd->line && data.cmd->line[i])
			ft_printf("%s\n", data.cmd[i++]);
		free_cmd(&data);
	}
	return (0);
}
