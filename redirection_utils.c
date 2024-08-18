/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:34:26 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/18 11:48:52 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// wrapper function for redirection of builtin commands
int	redirection_wrap_builtins(t_data *data, t_token *redir, char **envp)
{
	int	status;
	int	minilib_stdout;

	minilib_stdout = dup(STDOUT_FILENO);
	if (minilib_stdout < 0)
	{
		perror("dup");
		return (1);
	}
	if (handle_redirection(redir->next, redir->type) == -1)
	{
		if (dup2(minilib_stdout, STDOUT_FILENO) < 0)
			perror("dup2");
		close(minilib_stdout);
		return (1);
	}
	status = check_launch_builtins(data, data->token, envp);
	if (dup2(minilib_stdout, STDOUT_FILENO) < 0)
		perror("dup2");
	close(minilib_stdout);
	return (status);
}

// TODO: <<
// example of << delimiter: "cat <<'X' > t.txt		contentbla X"

// handle input/output redirection  (>, >>, <)
int	handle_redirection(t_token *fname, int type)
{
	int	fd;

	printf("type: %d, file name: %s\n", type, fname->value);
	// TODO: handle different file permissions
	if (type == TRUNC)
		fd = open(fname->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(fname->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(fname->value, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (-1);
	}
	if (type == TRUNC || type == APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			return (-1);
		}
	}
	else
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			close(fd);
			return (-1);
		}
	}
	close(fd);
	return (0);
}
