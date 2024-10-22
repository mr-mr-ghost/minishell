/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 12:09:32 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/19 12:09:32 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* echo_command: prints all the subsequent string							  */
/* if 1 str - prints new line, returns 1									  */
/* if 2 strs - if str2 = "-n", returns 1, else prints str2, returns 1		  */
/* if more strs - prints all strings, adds new line if str2 = "-n", returns 1 */
void	print_echo(t_token *echo_token)
{
	bool	first;

	first = true;
	while (echo_token && echo_token->type <= HEREDOC)
	{
		if (echo_token->type == ARG || echo_token->type == EMPTY)
		{
			if (first)
				first = false;
			else
				ft_putchar_fd(' ', 1);
			ft_putstr_fd(echo_token->value, 1);
		}
		echo_token = echo_token->next;
	}
}

int	echo_command(t_token *token)
{
	bool	n_flag;
	t_token	*echo_token;

	if (!token->next || token->next->type >= HEREDOC)
	{
		printf("\n");
		return (0);
	}
	echo_token = token->next;
	n_flag = false;
	if (!ft_memcmp(echo_token->value, "-n", 3))
	{
		if (!echo_token->next || echo_token->next->type >= HEREDOC)
			return (0);
		echo_token = echo_token->next;
		n_flag = true;
	}
	print_echo(echo_token);
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
