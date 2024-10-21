/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 12:09:32 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/21 11:54:34 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* void	print_echo(t_token *echo_token)
{
	while (echo_token && echo_token->type <= ARG)
	{
		if (echo_token->type == ARG)
			ft_putstr_fd(echo_token->value, 1);
		echo_token = echo_token->next;
		if (echo_token && echo_token->type <= ARG)
			ft_putchar_fd(' ', 1);
	}
} */

void	print_echo(t_token *echo_token)
{
	int	first;

	first = 1;
	while (echo_token && echo_token->type < PIPE)
	{
		if (echo_token->type == ARG)
		{
			if (first)
				first = 0;
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

	if (!token->next || token->next->type > PIPE)
	{
		printf("\n");
		return (0);
	}
	echo_token = token->next;
	n_flag = false;
	if (!ft_memcmp(echo_token->value, "-n", 3))
	{
		if (!echo_token->next || echo_token->next->type > PIPE)
			return (0);
		echo_token = echo_token->next;
		n_flag = true;
	}
	print_echo(echo_token);
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}
