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

#include "../minishell.h"

/* env_command: prints all the environment variables			*/
/* if 1 str - prints env vars, returns 1						*/
int	env_command(t_data *data, t_token *token)
{
	t_env	*enviro;
	char	*line;

	if (token->next && token->next->type == ARG)
		return (err_msg("env", data->token->next->value, "Invalid input", 1));
	enviro = data->env;
	while (enviro)
	{
		line = remove_quotes(enviro->line);
		if (!line)
			return (1);
		ft_putendl_fd(line, 1);
		free(line);
		enviro = enviro->next;
	}
	return (0);
}
