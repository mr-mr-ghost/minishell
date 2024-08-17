/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/17 16:37:32 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_sig;

void	print_tokens(t_token *token)
{
	while (token)
	{
		ft_printf("%s\n", token->value);
		ft_printf("type: %d\n\n", token->type);
		token = token->next;
	}
}

void	init_data(t_data *data)
{
	data->line = NULL;
	data->token = NULL;
	data->env = NULL;
	data->exit = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	disable_sigquit();
	init_data(&data);
	init_env(&data, envp);
	while (!data.exit)
	{
		sig_init();
		data.line = readline("Minishell> ");
		if (!data.line)
			break ;
		if (data.line[0] != '\0')
		{
			add_history(data.line);
			token_split(&data);
			//print_tokens(data.token);
			process_n_exec(&data, envp);
		}
		free_tokens(&data);
	}
	rl_clear_history();
	free_env(data.env);
	ft_printf("exit end of main\n");
	return (0);
}
