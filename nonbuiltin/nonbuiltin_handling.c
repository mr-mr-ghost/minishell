/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nonbuiltin_handling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:50:41 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/02 11:47:04 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	Function to execute non builtin command in a child process	*/
int	launch_nonbuiltins(t_data *data, t_token *cmdt, t_token *redirt)
{
	pid_t	pid;
	int		status;

	status = 0;
	signal_manager(sigint_handler_incmd, SA_RESTART);
	pid = fork();
	if (pid < 0)
		return (err_msg(NULL, NULL, strerror(errno), 1));
	else if (pid == 0)
		child_process(data, cmdt, redirt);
	else
		waitpid(pid, &status, 0);
	signal_manager(sigint_handler, SA_RESTART);
	if (g_sigint)
		return (130);
	return (WEXITSTATUS(status));
}
