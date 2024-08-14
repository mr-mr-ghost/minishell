/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:49:19 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/14 22:18:48 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
	return (1);
}

int	cd_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
	return (1);
}

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
	return (1);
}

int	export_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
	return (1);
}

int	unset_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
	return (1);
}

int	env_command(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		printf("%s\n", envp[i++]);
	return (1);
}

int	exit_command(t_data *data)
{
	int	i;

	if (data->cmd[1])
	{
		if (data->cmd[2])
		{
			printf("exit: too many arguments\n");
			return (1);
		}
		else
		{
			i = 0;
			while (data->cmd[1][i])
			{
				if (!ft_isdigit(data->cmd[1][i]))
				{
					printf("exit: numeric argument required\n");
					return (1);
				}
				i++;
			}
			i = ft_atoi(data->cmd[1]);
			rl_clear_history();
			free_data_content(data);
			free(data);
			exit(i);
		}
	}
	return (0);
}
