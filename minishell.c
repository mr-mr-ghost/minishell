/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/14 18:05:43 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		rl_clear_history();
		// TODO clear data or change data->status = 0
		exit(EXIT_SUCCESS);
	}
}

void	init_main(t_data **data, int argc)
{
	if (argc > 1)
		exit(EXIT_SUCCESS);
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
	{
		perror("setup handle_sigint()");
		exit(EXIT_FAILURE);
	}
	*data = malloc(sizeof(t_data));
	if (*data == NULL)
	{
		perror("malloc() t_data");
		exit(EXIT_FAILURE);
	}
	(*data)->status = 1;
	(*data)->cmd = NULL;
	(*data)->line = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	(void)argv;
	init_main(&data, argc);
	while (data->status)
	{
		data->line = readline("Minishell> ");
		if (!data->line || data->line[0] == '\0')
			continue ;
		add_history(data->line);
		data->cmd = ft_split(data->line, ' ');
		if (data->cmd == NULL || data->cmd[0] == NULL
			|| data->cmd[0][0] == '\0')
		{
			free_data_content(data);
			continue ;
		}
		data->status = execute_command(data, envp);
		free_data_content(data);
	}
	rl_clear_history();
	free(data);
	return (EXIT_SUCCESS);
}
