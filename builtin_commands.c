/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 15:49:19 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/13 17:12:34 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
}

void	cd_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
}

void	pwd_command(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("getcwd in pwd_command()");
		return ;
	}
	printf("%s\n", pwd);
	free(pwd);
}

void	export_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
}

void	unset_command(char **args)
{
	printf("builtin command TODO: %s\n", args[0]);
}

void	env_command(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		printf("%s\n", envp[i++]);
}
