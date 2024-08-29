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

void	bin_error(t_data *data, char *cmd)
{
	int	exit_status;

	exit_status = 0;
	exit_status = check_error(cmd);
	free_tokens(data);
	free_env(data->env);
	free_env(data->secret_env);
	exit(exit_status);
}

/*	Function to execute non builtin command in a child process	*/
int	launch_nonbuiltins(t_data *data, t_token *cmdt, t_token *redirt)
{
	pid_t	pid;
	char	*bin;
	int		status;

	bin = NULL;
	status = 0;
	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
	{
		bin = find_bin(data->env, cmdt->value);
		if (!bin)
			bin_error(data, cmdt->value);
		child_process(data, cmdt, redirt, bin);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid");
	}
	free(bin);
	if (g_sig.sigint || g_sig.sigquit)
		return (g_sig.exit_status);
	return (WEXITSTATUS(status));
}
