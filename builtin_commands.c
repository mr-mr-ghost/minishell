/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:49:19 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/14 23:34:36 by gklimasa         ###   ########.fr       */
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
	unsigned char	i;

	if (!data->cmd[1])
		return (0);
	if (!data->cmd[2])
	{
		i = 0;
		if (ft_strchr("-+", data->cmd[1][0]) != NULL && data->cmd[1][1] != '\0')
			i++;
		while (data->cmd[1][i] && ft_isdigit(data->cmd[1][i]))
			i++;
		if (data->cmd[1][i] != '\0')
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			return (1);
		}
		i = ft_atoi(data->cmd[1]);
		rl_clear_history();
		free_data_content(data);
		free(data);
		exit(i);
	}
	ft_putstr_fd("exit: too many arguments\n", 2);
	return (1);
}
