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
	while (echo_token && echo_token->type <= ARG)
	{
		if (echo_token->type == ARG)
			printf("%s", echo_token->value);
		echo_token = echo_token->next;
		if (echo_token && echo_token->type <= ARG)
			printf(" ");
	}
}

int	echo_command(t_token *token)
{
	int		n_flag;
	t_token	*echo_token;

	if (!token->next || token->next->type != ARG)
	{
		printf("\n");
		return (0);
	}
	echo_token = token->next;
	n_flag = 0;
	if (!ft_memcmp(echo_token->value, "-n", 3))
	{
		if (!echo_token->next || echo_token->next->type != ARG)
			return (0);
		echo_token = echo_token->next;
		n_flag = 1;
	}
	print_echo(echo_token);
	if (!n_flag)
		printf("\n");
	return (0);
}
