/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 13:48:33 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/18 13:48:35 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* env_command: prints all the environment variables			*/
/* if 1 str - prints env vars, returns 1						*/
/* if more strs - prints invalid command in STDERR, returns 1	*/
int	print_env(t_env *env)
{
	t_env	*enviro;
	char	*line;
	int		flag;

	enviro = env;
	while (enviro)
	{
		flag = 0;
		if (enviro->value && (ft_strchr(enviro->value, '\"') || ft_strchr(enviro->value, '\'')))
		{
			line = remove_quotes(enviro->line);
			if (!line)
				return (1);
			flag = 1;
		}
		else
			line = enviro->line;
		printf("%s\n", line);
		if (flag)
			free(line);
		enviro = enviro->next;
	}
	return (0);
}

int env_command(t_token *token, t_env *env)
{
	if (token->next && token->next->type == ARG)
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		return (0);
	}
	return (print_env(env));
}
