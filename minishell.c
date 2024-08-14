/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/14 11:54:57 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**read_and_split(char **cmd)
{
	char	*line;

	line = readline("Minishell> ");
	if (!line || line[0] == '\0') //bad? bash saves history of such line: ""
		return (NULL);
	add_history(line);
	cmd = ft_split(line, ' ');
	if (cmd == NULL || cmd[0] == NULL)
	{
		if (line)
			free(line);
		return (NULL);
	}
	free(line);
	return (cmd);
}

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		rl_clear_history(); // might need to clear cmd too somehow
		exit(EXIT_SUCCESS);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**cmd;
	int		status;

	if (argc > 1)
		return (EXIT_SUCCESS);
	(void)argv;
	status = 1;
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
		perror("setup handle_sigint()");
	while (status)
	{
		cmd = NULL;
		cmd = read_and_split(cmd);
		status = execute_command(cmd, envp);
		free_cmd(cmd);
	}
	rl_clear_history();
	return (EXIT_SUCCESS);
}
