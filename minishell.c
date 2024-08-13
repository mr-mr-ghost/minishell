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

void	print_tokens(t_token *token)
{
	while (token)
	{
		ft_printf("%s\n", token->value);
		ft_printf("type: %d\n", token->type);
		token = token->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	data.exit = 0;
	init_env(&data, envp);
	set_shell_lvl(data.env);
	data.token = (t_token *)malloc(sizeof(t_token));
	while (!data.exit)
	{
		data.line = readline("Minishell> ");
		token_split(&data);
		print_tokens(data.token);
		free_tokens(data.token);
	}
	free_env(data.env);
	return (0);
}
