/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:34:26 by gklimasa          #+#    #+#             */
/*   Updated: 2024/10/21 11:57:12 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// returns the first redirection token from the cmdt token
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

// redirection_wrap_builtins while cycle contents:
// launches redirection handler according to redirection type
// if redirection type is TRUNC or APPEND, calls handle_redirection
// if redirection type is INPUT, checks if file exists/readable and returns
// ignores HEREDOC, because it was dealt previousy (spaghetti regretti...)
// returns 0 on success, 1 on failure
int	call_redir_handler(t_token *redir, int minilib_stdout)
{
	if ((redir->type == TRUNC || redir-> type == APPEND)
		&& handle_redirection(redir->next, redir->type))
	{
		if (dup2(minilib_stdout, STDOUT_FILENO) < 0)
			err_msg(NULL, NULL, strerror(errno), 1);
		close(minilib_stdout);
		return (1);
	}
	else if (redir->type == INPUT)
	{
		if (access(redir->next->value, F_OK) == -1)
		{
			close(minilib_stdout);
			err_msg(NULL, redir->next->value, "No such file or directory", 1);
			return (1);
		}
		if (access(redir->next->value, R_OK) == -1)
		{
			close(minilib_stdout);
			err_msg(NULL, redir->next->value, "Permission denied", 1);
			return (1);
		}
	}
	return (0);
}

// wrapper function for redirection of builtin commands
// builtins need stdout to be dupped and then restored after redirection
int	redirection_wrap_builtins(t_data *data, t_token *cmdt, t_token *redir)
{
	int	status;
	int	minilib_stdout;

	status = 0;
	minilib_stdout = dup(STDOUT_FILENO);
	if (minilib_stdout < 0)
		return (err_msg(NULL, NULL, strerror(errno), 1));
	while (redir)
	{
		if (call_redir_handler(redir, minilib_stdout))
			return (1);
		redir = return_redirt(redir->next);
	}
	if (cmdt->type == CMD && status == 0)
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
	else if (type == INPUT)
		fd = open(fname->value, O_RDONLY);
	else
		return (err_msg(NULL, NULL, "Invalid redirection type", 1));
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
