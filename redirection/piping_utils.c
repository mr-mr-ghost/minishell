/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:10:08 by gklimasa          #+#    #+#             */
/*   Updated: 2024/10/19 21:48:59 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// similarities to launch_single_anycmd in execution.c
int	launch_cmd_inpipe(t_data *data, t_token *cmdt)
{
	t_token	*redirt;
	int		status;

	redirt = return_redirt(cmdt);
	if (!redirt)
	{
		status = check_launch_builtins(data, cmdt);
		if (status == -1)
		{
			child_process(data, cmdt, NULL);
			return (0);
		}
		else
			return (status);
	}
	else
	{
		if (is_cmd(cmdt->value, 0))
			return (hredir_builtin(data, cmdt, redirt, 1));
		else
			child_process(data, cmdt, redirt);
	}
	return (0);
}

// forks current cmd and cleans child's memory if cmd is builtin
int	pipe_fork(t_data *data, t_token *cmdt, int pfd[3][2], t_pvars *pvars)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
		return (err_msg(NULL, NULL, strerror(errno), -1));
	else if (pid == 0)
	{
		status = close_fd(pfd, pvars->htoken);
		if (pvars->hdoc)
		{
			free(pvars->hdoc);
			pvars->hdoc = NULL;
		}
		if (status >= 0)
			status = launch_cmd_inpipe(data, cmdt);
		rl_clear_history();
		free_tokens(data);
		free_env(data->env);
		free_env(data->secret_env);
		exit(status);
	}
	return (pid);
}

// function replaces current cmd's stdin/out with pipes and closes them:
// fd pairs work by opening one fd in parent and other fd in child process
// fd[0] pair is feeding prev cmd's result to current cmd (0 is in, 1 is out)
// fd[1] pair is feeding current cmd's result to next cmd (0 is in, 1 is out)
// fd[2] pair replaces fd[0] or stdin with heredoc (0 is in, 1 is out)
int	close_fd(int fd[3][2], t_token *htoken)
{
	int	status;

	status = 0;
	if (fd[0][0] != -1 && fd[0][1] != -1)
	{
		if (!htoken)
			status = dup2(fd[0][0], STDIN_FILENO);
		close(fd[0][0]);
		close(fd[0][1]);
	}
	if (htoken)
	{
		if (status >= 0)
			status = dup2(fd[2][0], STDIN_FILENO);
		close(fd[2][0]);
		close(fd[2][1]);
	}
	if (fd[1][0] != -1 && fd[1][1] != -1)
	{
		if (status >= 0)
			status = dup2(fd[1][1], STDOUT_FILENO);
		close(fd[1][0]);
		close(fd[1][1]);
	}
	return (status);
}

// edits pipefd[3][2] - affected pair is *pipefd
// if isclose = 1, then *pipefd is closed
// else if pair *pipefd2 is not NULL, then it's values are set on *pipefd
// else *pipefd values are set to int value
void	edit_pipeset(int *pipefd, int *pipefd2, int value, int isclose)
{
	if (isclose)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	else if (pipefd2)
	{
		pipefd[0] = pipefd2[0];
		pipefd[1] = pipefd2[1];
	}
	else
	{
		pipefd[0] = value;
		pipefd[1] = value;
	}
}

// opens pipe, writes the fds into the pipe matrix
// returns 1 on success
// returns 0 on failure and frees heredoc before return
int	is_pipe(char *heredoc, int *fd, int *status)
{
	if (pipe(fd) == -1)
	{
		if (heredoc)
			free(heredoc);
		*status = err_msg(NULL, NULL, strerror(errno), -1);
		return (0);
	}
	return (1);
}
