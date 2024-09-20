/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:49:19 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/16 02:02:23 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// pwd_command: prints the working directory
// any amount of strs - prints pwd and returns 1
// upon getcwd() fail - prints invalid command in STDERR, returns 1
int	pwd_command(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (err_msg("pwd", NULL, strerror(errno), 1));
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (0);
}
