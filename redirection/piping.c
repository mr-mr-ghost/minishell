/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 11:02:46 by gklimasa          #+#    #+#             */
/*   Updated: 2024/10/18 15:14:36 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// initiate variables for the call_pipe function
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

// edits pipeset accordingly and launches pipe_fork (to fork the current cmd):
// if current cmd is first, pfd[0] (stdin replace pair) is set to -1, to not use
// if current cmd is in between other cmds, pfd[0] and pfd[1] are already set
// if current cmd is last, pfd[1] (stdout replace pair) is set to -1, to not use
void	prep_pfork(t_data *data, t_token *currt, t_token *next, t_pvars *pvars)
{
	if (currt->prev == NULL)
	{
		edit_pipeset(pvars->pfd[0], NULL, -1, 0);
		pvars->status = pipe_fork(data, currt, pvars->pfd, pvars->hdoc);
	}
	else if (next)
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
		if (nextt && is_pipe(pvars.hdoc, pvars.pfd[1]) == 0)
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
