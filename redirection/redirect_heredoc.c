/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:37:15 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/19 22:22:13 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_heredoc(t_data *data, char *delimiter)
{
	char	*heredoc;
	char	*hereline;

	signal_manager(sigint_handler_incmd, 0);
	ft_putstr_fd("> ", 1);
	heredoc = NULL;
	hereline = get_next_line(0);
	while (hereline)
	{
		if (ft_strncmp(hereline, delimiter, ft_strlen(delimiter)) == 0
			&& hereline[ft_strlen(delimiter)] == '\n')
			break ;
		else
			ft_putstr_fd("> ", 1);
		heredoc = join_strings(data, heredoc, hereline);
		hereline = get_next_line(0);
	}
	if (g_sigint || !hereline
		|| ft_strncmp(hereline, delimiter, ft_strlen(delimiter)))
		heredoc = heredoc_error(delimiter, heredoc);
	if (hereline)
		free(hereline);
	signal_manager(sigint_handler, SA_RESTART);
	return (heredoc);
}

int	process_heredoc(t_data *data, t_token *cmdt, int *fd, char *heredoc)
{
	pid_t	pid;
	int		duperr;

	signal_manager(sigint_handler_incmd, SA_RESTART);
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (err_msg(NULL, NULL, strerror(errno), 1));
	}
	if (pid == 0)
	{
		close(fd[1]);
		duperr = dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (heredoc)
			free(heredoc);
		if (duperr >= 0)
			child_process(data, cmdt, return_redirt(cmdt));
		else
			child_cleanexit(data, NULL, NULL, NULL);
	}
	return (0);
}

int	handle_heredoc_error(char *msg, char *heredoc, int code)
{
	if (msg)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putendl_fd(msg, 2);
	}
	if (heredoc)
		free(heredoc);
	return (code);
}

/* example of << delimiter: "cat <<'X' > t.txt		contentbla X"*/
int	handle_heredoc(t_data *data, t_token *cmdt, t_token *hdtoken)
{
	char	*heredoc;
	int		pipefd[2];
	int		status;

	heredoc = get_heredoc(data, hdtoken->next->value);
	if (g_sigint)
		return (130);
	if (!hdtoken->prev || hdtoken->prev->type == PIPE)
		return (handle_heredoc_error(NULL, heredoc, 0));
	if (pipe(pipefd) == -1)
		return (handle_heredoc_error(strerror(errno), heredoc, 1));
	if (process_heredoc(data, cmdt, pipefd, heredoc) != 0)
		return (handle_heredoc_error(NULL, heredoc, 1));
	close(pipefd[0]);
	ft_putstr_fd(heredoc, pipefd[1]);
	if (heredoc)
		free(heredoc);
	close(pipefd[1]);
	wait(&status);
	signal_manager(sigint_handler, SA_RESTART);
	return (WEXITSTATUS(status));
}

int	hredir_builtin(t_data *data, t_token *cmdt, t_token *redir, int ispipe)
{
	char	*heredoc;
	t_token	*hdtoken;
	int		ret;

	heredoc = NULL;
	if (!ispipe)
	{
		hdtoken = return_lastheredoct(cmdt);
		if (hdtoken)
			heredoc = get_heredoc(data, hdtoken->next->value);
		if (heredoc)
			free(heredoc);
	}
	if (g_sigint)
		return (130);
	while (redir && redir->type == HEREDOC)
		redir = return_redirt(redir->next);
	if (!redir)
		ret = check_launch_builtins(data, cmdt);
	else
		ret = redirection_wrap_builtins(data, cmdt, redir);
	return (ret);
}
