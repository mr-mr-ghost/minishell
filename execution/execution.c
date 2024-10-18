/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:57:01 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/18 18:18:35 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// function returns n_th token counting from *token, or NULL upon failure
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

// function returns first heredoc (or NULL) of the current command
t_token	*return_1stheredoct(t_token *cmdt)
{
	int		count;
	t_token	*redirt;

	count = count_args(cmdt, TRUNC);
	redirt = get_nth_token(cmdt, count);
	while (redirt && redirt->type >= TRUNC && redirt->type <= INPUT)
	{
		count = count_args(redirt->next, TRUNC);
		redirt = get_nth_token(redirt->next, count);
	}
	if (redirt && redirt->type == HEREDOC)
		return (redirt);
	else
		return (NULL);
}

int	launch_single_anycmd(t_data *data, t_token *cmdt)
{
	t_token	*redirt;
	t_token	*hdtoken;
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
		hdtoken = return_1stheredoct(cmdt);
		if (is_cmd(cmdt->value, 0))
			return (hredir_builtin(data, cmdt, redirt, 0));
		else if (hdtoken)
			return (handle_heredoc(data, cmdt, hdtoken));
		else
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
		data->exit_code = call_pipe(data, data->token, nextt->next);
	else
		data->exit_code = err_msg(NULL, data->token->value,
				"Command not found", 127);
}
