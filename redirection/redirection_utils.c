/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:34:26 by gklimasa          #+#    #+#             */
/*   Updated: 2024/10/09 13:19:41 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* wrapper function for redirection of builtin commands*/
int	redirection_wrap_builtins(t_data *data, t_token *cmdt, t_token *redir)
{
	int	status;
	int	minilib_stdout;

	minilib_stdout = dup(STDOUT_FILENO);
	if (minilib_stdout < 0)
		return (err_msg(NULL, NULL, strerror(errno), 1));
	while (redir)
	{
		if (redir->type >= TRUNC && redir-> type <= INPUT &&
			handle_redirection(redir->next, redir->type))
		{
			if (dup2(minilib_stdout, STDOUT_FILENO) < 0)
				err_msg(NULL, NULL, strerror(errno), 1);
			close(minilib_stdout);
			return (1);
		}
		redir = return_redirt(redir->next);
	}
	status = 0;
	if (cmdt->type == CMD)
		status = check_launch_builtins(data, cmdt);
	if (dup2(minilib_stdout, STDOUT_FILENO) < 0)
		err_msg(NULL, NULL, strerror(errno), 1);
	close(minilib_stdout);
	return (status);
}

/* handles input/output redirection (>, >>, <)*/
/* opens a file and sets it as the new stdout or stdin*/
/* returns 0 on success, -1 on failure of open or dup2*/
int	handle_redirection(t_token *fname, int type)
{
	int	fd;
	int	dupstatus;

	if (type == TRUNC)
		fd = open(fname->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == APPEND)
		fd = open(fname->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(fname->value, O_RDONLY);
	if (fd < 0)
		return (err_msg(NULL, fname->value, strerror(errno), 1));
	if (type == TRUNC || type == APPEND)
		dupstatus = dup2(fd, STDOUT_FILENO);
	else
		dupstatus = dup2(fd, STDIN_FILENO);
	close(fd);
	if (dupstatus < 0)
		return (err_msg(NULL, NULL, strerror(errno), 1));
	return (0);
}
