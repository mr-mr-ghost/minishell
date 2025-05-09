/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:39:53 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/08 13:26:46 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* exit_command: frees data and exits the program						*/
/* if 1 str - returns status 0 to exit command by ending main loop		*/
/* if 2 strs - checks if str2 is nbr and exits program with STDERR=nbr	*/
/* if more strs - prints invalid command in STDERR, returns 1			*/
int	validate_exit(char *arg)
{
	size_t	i;
	int		nbr;

	i = 0;
	if (ft_strchr("-+", arg[0]) && arg[1] != '\0')
		i++;
	while (arg[i] && ft_isdigit(arg[i]))
		i++;
	if (arg[i] != '\0')
		return (err_msg("exit", arg, "numeric argument required", 2));
	nbr = ft_atoi(arg);
	if (nbr < 0)
		nbr = 256 + (nbr % 256);
	else
		nbr = nbr % 256;
	return (nbr);
}

int	exit_command(t_data *data, t_token *token)
{
	int		exit_status;
	t_token	*exit_token;

	if (!token->next || token->next->type >= TRUNC)
	{
		data->end = true;
		return (0);
	}
	exit_token = token->next;
	if (exit_token->next && exit_token->next->type <= ARG
		&& check_numeric(exit_token->value))
	{
		ft_putendl_fd("exit", 1);
		return (err_msg("exit", NULL, "too many arguments", 1));
	}
	exit_status = validate_exit(exit_token->value);
	data->end = true;
	return (exit_status);
}
