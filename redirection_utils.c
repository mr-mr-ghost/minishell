/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:34:26 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/17 16:54:07 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// wrapper function for redirection of builtin commands
int	redirection_wrap_builtins(t_data *data, t_token *redir)
{
	int	status;
	int	minilib_stdout;

	minilib_stdout = dup(STDOUT_FILENO);
	if (handle_redirection(redir->next, redir->type) == -1)
	{
		dup2(minilib_stdout, STDOUT_FILENO);
		return (1);
	}
	status = check_launch_builtins(data);
	dup2(minilib_stdout, STDOUT_FILENO);
	return (status);
}

// TODO: <<
// example of << delimiter: "cat <<'X' > t.txt		contentbla X"

// handle input/output redirection  (>, >>, <)
int	handle_redirection(t_token *fname, int type)
{
	int		fd;

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
		dup2(fd, STDOUT_FILENO);
	else
		dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}
