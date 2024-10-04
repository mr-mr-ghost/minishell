/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:57:01 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/03 13:24:00 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Function to extract commands, check if they're builtin, launch accordingly*/
t_token	*get_nth_token(t_token *token, int n)
{
	t_token	*tmp;
	int		i;

	if (!token)
		return (NULL);
	i = 0;
	tmp = token;
	while (tmp && i < n)
	{
		i++;
		tmp = tmp->next;
	}
	if (i < n)
		return (NULL);
	return (tmp);
}

t_token	*return_redirt(t_token *cmdt)
{
	int		count;
	t_token	*redirt;

	count = count_args(cmdt, TRUNC);
	redirt = get_nth_token(cmdt, count);
	if (redirt && redirt->type >= TRUNC && redirt->type <= HEREDOC)
		return (redirt);
	else
		return (NULL);
}

int	launch_single_anycmd(t_data *data, t_token *cmdt)
{
	t_token	*redirt;
	int		status;

	redirt = return_redirt(cmdt);
	if (!redirt)
	{
		status = check_launch_builtins(data, cmdt);
		if (status == -1)
			status = launch_nonbuiltins(data, cmdt, NULL);
		return (status);
	}
	else
	{
		if (redirt->type == HEREDOC)
			return (handle_heredoc(data, cmdt, redirt));
		if (is_cmd(cmdt->value, 0)
			|| (cmdt->type >= TRUNC && cmdt->type <= INPUT))
			return (redirection_wrap_builtins(data, cmdt, redirt));
		return (launch_nonbuiltins(data, cmdt, redirt));
	}
}

void	process_n_exec(t_data *data)
{
	int		count;
	t_token	*nextt;

	if (!data->token)
		return ;
	count = count_args(data->token, PIPE);
	nextt = get_nth_token(data->token, count);
	if (!nextt)
		data->exit_code = launch_single_anycmd(data, data->token);
	else if (nextt->type == PIPE && nextt->next)
		data->exit_code = call_pipe(data, data->token);
	else
		data->exit_code = err_msg(NULL, data->token->value,
				"Command not found", 127);
}
