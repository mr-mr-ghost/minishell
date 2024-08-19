/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:49:19 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/16 02:02:23 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_command(t_token *token)
{
	printf("builtin command TODO: %s\n", token->value);
	return (0);
}

// pwd_command: prints the working directory
// any amount of strs - prints pwd and returns 1
// upon getcwd() fail - prints invalid command in STDERR, returns 1
int	pwd_command(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd in pwd_command()");
		return (0);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}

// exit_command: frees data and exits the program
// if 1 str - returns status 0 to exit command by ending main loop
// if 2 strs - checks if str2 is nbr and exits program with STDERR=nbr
// if more strs - prints invalid command in STDERR, returns 1
int	exit_command(t_data *data, t_token *token)
{
	unsigned char	i;
	t_token			*exit_token;

	if (!token->next)
		return (1);
	exit_token = token->next;
	if (!exit_token->next)
	{
		i = 0;
		if (ft_strchr("-+", exit_token->value[0]) != NULL && exit_token->value[1] != '\0')
			i++;
		while (exit_token->value[i] && ft_isdigit(exit_token->value[i]))
			i++;
		if (exit_token->value[i] != '\0')
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			return (0);
		}
		i = ft_atoi(exit_token->value);
		rl_clear_history();
		free_tokens(data);
		free_env(data->env);
		ft_putstr_fd("exit\n", 2);
		exit(i);
	}
	ft_putstr_fd("exit: too many arguments\n", 2);
	return (0);
}
