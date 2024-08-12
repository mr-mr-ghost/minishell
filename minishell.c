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

char	**read_and_split(char *line)
{
	char	**cmd;

	cmd = ft_split(line, ' ');
	if (cmd == NULL || cmd[0] == NULL)
		return (NULL);
	free(line);
	return (cmd);
}

//void	token_split(t_data *data)
//{
//	/* TODO: Split line into tokens into a linked list */
//	/* TODO: Determine the type of token */
//}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	init_env(&data, envp);
	set_shell_lvl(data.env);
	data.token = (t_token *)malloc(sizeof(t_token));
	while (1)
	{
		data.line = readline("Minishell> ");
		data.cmd = read_and_split(data.line);
//		token_split(&data);
	}
	return (0);
}
