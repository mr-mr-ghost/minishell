/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nonbuiltin_handling.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:50:41 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/26 15:50:41 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*	Function to execute non builtin command in a child process	*/
int	launch_nonbuiltins(t_data *data, t_token *cmdt, t_token *redirt)
{
	pid_t	pid;
	int		status;

	status = 0;
	g_sig.in_cmd = true;
	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
		child_process(data, cmdt, redirt);
	else
		waitpid(pid, &status, 0);
	if (g_sig.sigint)
		return (130);
	return (WEXITSTATUS(status));
}
