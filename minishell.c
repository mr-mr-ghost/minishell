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

t_sig	g_sig;

void	exit_shell(t_data *data)
{
	ft_putendl_fd("exit", 1);
	rl_clear_history();
	free_env(data->env);
	free_env(data->secret_env);
}

void	init_data(t_data *data)
{
	data->line = NULL;
	data->token = NULL;
	data->env = NULL;
	data->secret_env = NULL;
	data->exit_code = 0;
	data->end = false;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	signal_manager(sigint_handler, SA_RESTART);
	init_data(&data);
	init_env(&data, envp);
	while (!data.end)
	{
		sig_init();
		data.line = read_line(data.env);
		if (!data.line)
			break ;
		if (token_split(&data))
			continue ;
		if (g_sig.sigint)
			data.exit_code = 130;
		else if (!data.token)
			data.exit_code = 0;
		process_n_exec(&data);
		free_tokens(&data);
	}
	exit_shell(&data);
	return (data.exit_code);
}
