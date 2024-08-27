/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:57:01 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/26 13:57:01 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Function to extract commands, check if they're builtin, launch accordingly*/
void	process_n_exec(t_data *data)
{
	t_token	*token;

	if (!data->token)
		return ;
	token = data->token;
	if ((!token->prev || token->type == ARG) && ft_strstr(token->value, "="))
		data->exit_code = handle_declaration(data->secret_env, token);
	else if (token->type == CMD && is_cmd(token->value, 0))
		data->exit_code = check_launch_builtins(data, token);
	else if (token->type == CMD)
		data->exit_code = launch_nonbuiltins(data, token, NULL);
	else
		data->exit_code = err_msg(NULL, data->token->value,
				"command not found", 127);
}
