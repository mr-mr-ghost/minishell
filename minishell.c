/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/14 16:59:12 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* char	**read_and_split(t_data *data)
{
	data->line = readline("Minishell> ");
	if (!data->line || data->line[0] == '\0') //bad? bash saves history of such line: ""
		return (NULL);
	add_history(data->line);
	data->cmd = ft_split(data->line, ' ');
	if (data->cmd == NULL || data->cmd[0] == NULL)
	{
		if (data->line)
			free(data->line);
		return (NULL);
	}
	return (data);
} */

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		rl_clear_history();
		// TODO clear data or change data->status = 0
		exit(EXIT_SUCCESS);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	if (argc > 1)
		return (EXIT_SUCCESS);
	(void)argv;
	data = malloc(sizeof(t_data));
	if (!data)
	{
		perror("malloc() t_data");
		return (0);
	}
	data->status = 1;
	data->cmd = NULL;
	data->line = NULL;
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
		perror("setup handle_sigint()");
	while (data->status)
	{
		data->line = readline("Minishell> ");
		if (!data->line) // bash saves history of such line: ""
			continue ;
		add_history(data->line);
		data->cmd = ft_split(data->line, ' ');
		if (data->cmd == NULL || data->cmd[0] == NULL
			|| data->cmd[0][0] == '\0')
		{
			free_data_content(data);
			continue ;
		}
		data->status = execute_command(data->cmd, envp);
		free_data_content(data);
	}
	rl_clear_history();
	free(data);
	return (EXIT_SUCCESS);
}
