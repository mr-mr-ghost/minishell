/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:49:19 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/16 14:03:50 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo_command: prints all the subsequent strings
// if 1 str - prints new line, returns 1
// if 2 strs - if str2 = "-n", returns 1, else prints str2, returns 1
// if more strs - prints all strings, adds new line if str2 = "-n", returns 1
int	echo_command(char **args)
{
	int	i;
	int	n_flag;

	if (!args[1])
	{
		printf("\n");
		return (0);
	}
	if (ft_strncmp(args[1], "-n", 3) == 0)
	{
		if (!args[2])
			return (0);
		i = 2;
		n_flag = 1;
	}
	else
	{
		i = 1;
		n_flag = 0;
	}
	while (args[i])
	{
		printf("%s", args[i++]);
		if (args[i])
			printf(" ");
	}
	if (!n_flag)
		printf("\n");
	return (0);
}

int	cd_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
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

int	export_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
	return (0);
}

int	unset_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
	return (0);
}

// env_command: prints all the environment variables
// if 1 str - prints env vars, returns 1
// if more strs - prints invalid command in STDERR, returns 1
int	env_command(char **args, t_env *env)
{
	t_env	*tmp;

	if (!args[1])
	{
		tmp = env;
		printf("deal with our env");
		while (tmp)
		{
			printf("%s\n", tmp->value);
			tmp = tmp->next;
		}
		return (0);
	}
	ft_putstr_fd("env: too many arguments\n", 2);
	return (0);
}

// exit_command: frees data and exits the program
// if 1 str - returns status 0 to exit command by ending main loop
// if 2 strs - checks if str2 is nbr and exits program with STDERR=nbr
// if more strs - prints invalid command in STDERR, returns 1
int	exit_command(t_data *data, char **args)
{
	unsigned char	i;

	if (!args[1])
		return (1);
	if (!args[2])
	{
		i = 0;
		if (ft_strchr("-+", args[1][0]) != NULL && args[1][1] != '\0')
			i++;
		while (args[1][i] && ft_isdigit(args[1][i]))
			i++;
		if (args[1][i] != '\0')
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			return (0);
		}
		i = ft_atoi(args[1]);
		rl_clear_history();
		free_cmd(args);
		free_tokens(data);
		free_env(data->env);
		exit(i);
	}
	ft_putstr_fd("exit: too many arguments\n", 2);
	return (0);
}
