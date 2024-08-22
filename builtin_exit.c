/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:39:53 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/21 13:39:53 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* exit_command: frees data and exits the program						*/
/* if 1 str - returns status 0 to exit command by ending main loop		*/
/* if 2 strs - checks if str2 is nbr and exits program with STDERR=nbr	*/
/* if more strs - prints invalid command in STDERR, returns 1			*/
int	validate_exit(char *arg)
{
	size_t	i;

	i = 0;
	if (ft_strchr("-+", arg[0]) != NULL && arg[1] != '\0')
		i++;
	while (arg[i] && ft_isdigit(arg[i]))
		i++;
	if (arg[i] != '\0')
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		return (2);
	}
	return (ft_atoi(arg));
}

int	exit_command(t_data *data, t_token *token)
{
	int		exit_status;
	t_token	*exit_token;

	if (!token->next || token->next->type != ARG)
	{
		data->loop = true;
		return (0);
	}
	exit_token = token->next;
	if (!exit_token->next)
	{
		exit_status = validate_exit(exit_token->value);
		data->loop = true;
		return (exit_status);
	}
	ft_putstr_fd("exit: too many arguments\n", 2);
	return (1);
}
