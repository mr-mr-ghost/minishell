/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 11:02:46 by gklimasa          #+#    #+#             */
/*   Updated: 2024/10/18 14:53:52 by gklimasa         ###   ########.fr       */
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

// opens pipe, writes the fds into the pipe matrix
// returns 1 on success
// returns 0 on failure and frees heredoc before return
int	is_pipe(char *heredoc, int *fd, int status)
{
	if (pipe(fd) == -1)
	{
		if (heredoc)
			free(heredoc);
		status = err_msg(NULL, NULL, strerror(errno), -1);
		return (0);
	}
	return (1);
}

// check for heredoc token, retrieve heredoc char, open heredoc pipe
// on success return heredoc file
// on failure return NULL
char	*set_heredoc(t_data *data, t_token *currentt, t_pvars *pvars)
{
	char	*heredoc;

	pvars->htoken = return_1stheredoct(currentt);
	if (pvars->htoken)
	{
		heredoc = get_heredoc(data, pvars->htoken->next->value);
		if (!heredoc || g_sigint)
			return (NULL);
		if (!is_pipe(heredoc, pvars->pfd[2], pvars->status))
			return (NULL);
		return (heredoc);
	}
	return (NULL);
}

// send heredoc through heredoc pipe, free it and set it to NULL
void	send_clean_heredoc(t_pvars *pvars)
{
	if (pvars->hdoc)
	{
		ft_putstr_fd(pvars->hdoc, pvars->pfd[2][1]);
		edit_pipeset(pvars->pfd[2], NULL, 0, 1);
		free (pvars->hdoc);
		pvars->hdoc = NULL;
	}
	pvars->htoken = NULL;
}

// get next command after pipe
t_token	*get_next_cmd(t_token *currentt)
{
	t_token	*nextt;

	nextt = get_nth_token(currentt, count_args(currentt, PIPE));
	if (nextt && nextt->type == PIPE && nextt->next)
		return (nextt->next);
	else
		return (NULL);
}

void	init_pvars(t_pvars *pvars)
{
	pvars->status = 0;
	pvars->pid = 0;
	pvars->htoken = NULL;
	pvars->hdoc = NULL;
	pvars->pfd[0][0] = -1;
	pvars->pfd[0][1] = -1;
	pvars->pfd[1][0] = -1;
	pvars->pfd[1][1] = -1;
	pvars->pfd[2][0] = -1;
	pvars->pfd[2][1] = -1;
}

// edits pipeset accordingly and launches pipe_fork (to fork the current cmd):
// if current cmd is first, pfd[0] (stdin replace pair) is set to -1, to not use
// if current cmd is in between other cmds, pfd[0] and pfd[1] are already set
// if current cmd is last, pfd[1] (stdout replace pair) is set to -1, to not use
void	prep_pfork(t_data *data, t_token *currt, t_token *nextt, t_pvars *pvars)
{
	if (currt->prev == NULL)
	{
		edit_pipeset(pvars->pfd[0], NULL, -1, 0);
		pvars->status = pipe_fork(data, currt, pvars->pfd, pvars->hdoc);
	}
	else if (nextt)
		pvars->status = pipe_fork(data, currt, pvars->pfd, pvars->hdoc);
	else
	{
		edit_pipeset(pvars->pfd[1], NULL, -1, 0);
		pvars->pid = pipe_fork(data, currt, pvars->pfd, pvars->hdoc);
	}
}

// prepare variables for the next while iteration in call_pipe:
// if current command is first, then close the pfd[0] (stdin)
// if there is next command, then put pfd[1] into pfd[0]
//		setting up this command's output as next command's input
// if there's heredoc set up - send it out, free it and close heredoc's pipe
// setup next currentt cmd as next or NULL, if there are failure indicators
t_token	*prep_next_iter(t_token *currentt, t_token *nextt, t_pvars *pvars)
{
	if (currentt->prev != NULL)
		edit_pipeset(pvars->pfd[0], NULL, 0, 1);
	if (nextt)
		edit_pipeset(pvars->pfd[0], pvars->pfd[1], 0, 0);
	send_clean_heredoc(pvars);
	if (pvars->status >= 0 || pvars->pid >= 0)
		return (nextt);
	else
		return (NULL);
}

// runs while function to go through all the piped cmds upon each iteration
int	call_pipe(t_data *data, t_token *currentt, t_token	*nextt)
{
	t_pvars	pvars;

	signal_manager(sigint_handler_incmd, SA_RESTART);
	init_pvars(&pvars);
	while (currentt)
	{
		nextt = get_next_cmd(currentt);
		pvars.hdoc = set_heredoc(data, currentt, &pvars);
		if (return_1stheredoct(currentt) && (g_sigint || pvars.status < 0))
			break ;
		if (nextt && is_pipe(pvars.hdoc, pvars.pfd[1], pvars.status) == 0)
			break ;
		prep_pfork(data, currentt, nextt, &pvars);
		currentt = prep_next_iter(currentt, nextt, &pvars);
	}
	if (pvars.pfd[0][0] != -1 && pvars.pfd[0][1] != -1)
		edit_pipeset(pvars.pfd[0], NULL, 0, 1);
	waitpid(pvars.pid, &(pvars.status), 0);
	if (g_sigint)
		pvars.status = 0x8200;
	while (wait(NULL) > 0)
		;
	signal_manager(sigint_handler, SA_RESTART);
	return (WEXITSTATUS(pvars.status));
}
