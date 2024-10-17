/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 11:02:46 by gklimasa          #+#    #+#             */
/*   Updated: 2024/10/17 16:29:48 by gklimasa         ###   ########.fr       */
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
			child_process(data, cmdt, NULL);
		else
			return (status);
	}
	else
	{
		if (is_cmd(cmdt->value, 0))
			return (handle_heredoc_builtins(data, cmdt, NULL));
		else
			child_process(data, cmdt, redirt);
	}
	return (0);
}

// function replaces current cmd's stdin/out with pipes and closes them:
// fd pairs work by opening one fd in parent and other fd in child process
// fd[0] pair is feeding prev cmd's result to current cmd (0 is in, 1 is out)
// fd[1] pair is feeding current cmd's result to next cmd (0 is in, 1 is out)
// fd[2] pair replaces fd[0] or stdin with heredoc (0 is in, 1 is out)
int	close_fd(int fd[3][2], char *heredoc)
{
	int	status;

	status = 0;
	if (fd[0][0] != -1 && fd[0][1] != -1)
	{
		if (!heredoc)
			status = dup2(fd[0][0], STDIN_FILENO);
		close(fd[0][0]);
		close(fd[0][1]);
	}
	if (heredoc)
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

// forks current cmd and cleans child's memory if that cmd is builtin ?
int	pipe_fork(t_data *data, t_token *cmdt, int pipefd[3][2], char *heredoc)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid < 0)
		return (err_msg(NULL, NULL, strerror(errno), -1));
	else if (pid == 0)
	{
		status = close_fd(pipefd, heredoc);
		if (heredoc)
		{
			free(heredoc);
			heredoc = NULL;
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

char	*set_pipevars(int pipefd[3][2], int *pid)
{
	pipefd[0][0] = -1;
	pipefd[0][1] = -1;
	pipefd[1][0] = -1;
	pipefd[1][1] = -1;
	pipefd[2][0] = -1;
	pipefd[2][1] = -1;
	*pid = 0;
	return (NULL);
}

char	*check_set_heredoc(t_data *data, t_token *currentt, int *hfd, int *sptr)
{
	t_token *hdtoken;
	char	*heredoc;

	hdtoken = return_1stheredoct(currentt);
	if (hdtoken)
	{
		heredoc = get_heredoc(data, hdtoken->next->value);
		if (!heredoc || g_sigint)
			return (NULL);
		if (pipe(hfd) == -1)
		{
			if (heredoc)
				free(heredoc);
			*sptr = err_msg(NULL, NULL, strerror(errno), -1);
			return (NULL);
		}
		return (heredoc);
	}
	return (NULL);
}

void	send_clean_heredoc(char *heredoc, int *heredocfd)
{
	if (heredoc)
	{
		ft_putstr_fd(heredoc, heredocfd[1]);
		edit_pipeset(heredocfd, NULL, 0, 1);
		if (heredoc)
			free (heredoc);
		heredoc = NULL;
	}
}

int	call_pipe(t_data *data, t_token *currentt, t_token	*nextt)
{
	char	*heredoc;
	int		pipefd[3][2]; //pipefd[0] in, pipefd[1] out, pipefd[2] heredoc
	int		status;
	pid_t	pid;

	signal_manager(sigint_handler_incmd, SA_RESTART);
	heredoc = set_pipevars(pipefd, &pid);
	while (currentt)
	{
		nextt = get_nth_token(currentt, count_args(currentt, PIPE));
		if (nextt && nextt->type == PIPE && nextt->next)
			nextt = nextt->next;
		else
			nextt = NULL;
		heredoc = check_set_heredoc(data, currentt, pipefd[2], &status);
		if (return_1stheredoct(currentt) && (g_sigint || status < 0))
			break ;
		if (nextt && pipe(pipefd[1]) == -1) // pipe fail
		{
			if (heredoc)
				free(heredoc);
			status = err_msg(NULL, NULL, strerror(errno), -1);
			break ;
		}
		if (currentt->prev == NULL) // first command fork
		{
			edit_pipeset(pipefd[0], NULL, -1, 0);
			status = pipe_fork(data, currentt, pipefd, heredoc);
		}
		else if (nextt) // mid command fork
			status = pipe_fork(data, currentt, pipefd, heredoc);
		else // end command fork
		{
			edit_pipeset(pipefd[1], NULL, -1, 0);
			pid = pipe_fork(data, currentt, pipefd, heredoc);
		}
		if (currentt->prev != NULL) // Close the previous pipe in the parent
			edit_pipeset(pipefd[0], NULL, 0, 1);
		if (nextt) // Move the current pipe to prev_pipefd for the next iteration
			edit_pipeset(pipefd[0], pipefd[1], 0, 0);
		currentt = nextt; // set next command as current command
		send_clean_heredoc(heredoc, pipefd[2]);
		//if (status != 0 || pid < 0)
		if (status < 0 || pid < 0) // errors from forks?
			currentt = NULL;
	}
	if (pipefd[0][0] != -1 && pipefd[0][1] != -1) // Close the last pipe in the parent process
		edit_pipeset(pipefd[0], NULL, 0, 1);
	waitpid(pid, &status, 0);
	if (g_sigint)
		status = 0x8200;
	while (wait(NULL) > 0) // Wait for all child processes
		;
	signal_manager(sigint_handler, SA_RESTART);
	return (WEXITSTATUS(status));
}
