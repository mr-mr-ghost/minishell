/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:37:15 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/04 15:24:34 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_heredoc(t_data *data, char *delimiter)
{
	char	*heredoc;
	char	*hereline;

	signal_manager(heredoc_sig_handler, 0);
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
	if (g_sig.sigint || !hereline
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
	t_token	*redirt;

	redirt = return_redirt(cmdt);

	pid = fork();
	g_sig.in_cmd = true;
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (err_msg(NULL, NULL, strerror(errno), 1));
	}
	if (pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (heredoc)
			free(heredoc);
		child_process(data, cmdt, redirt);
	}
	return (0);
}

/* example of << delimiter: "cat <<'X' > t.txt		contentbla X"*/
int	handle_heredoc(t_data *data, t_token *cmdt, t_token *hdtoken)
{
	char	*heredoc;
	int		pipefd[2];
	int		status;

	if (g_sig.sigint) // ctrl + c
		return (130);
	heredoc = get_heredoc(data, hdtoken->next->value);

	// if no heredoc deal with that?

	if (pipe(pipefd) == -1)
	{
		if (heredoc)
			free(heredoc);
		return (err_msg(NULL, NULL, strerror(errno), 1));
	}
	if (process_heredoc(data, cmdt, pipefd, heredoc) != 0)
	{
		if (heredoc)
			free(heredoc);
		return (1);
	}
	close(pipefd[0]);
	// Program receives signal SIGPIPE (dies?) from commands who dont read STDIN
	ft_putstr_fd(heredoc, pipefd[1]);
	if (heredoc)
		free(heredoc);
	close(pipefd[1]);
	wait(&status);
	return (WEXITSTATUS(status));
}

int	handle_heredoc_builtins(t_data *data, t_token *cmdt, t_token *hdtoken)
{
	char	*heredoc;
	t_token	*redirt;
	int		ret;

	if (g_sig.sigint) // ctrl + c
		return (130);
	heredoc = get_heredoc(data, hdtoken->next->value);
	if (heredoc)
		free(heredoc);

	redirt = return_redirt(cmdt);
	while (redirt && redirt->type == HEREDOC)
		redirt = return_redirt(redirt->next);

	if (!redirt)
		ret = check_launch_builtins(data, cmdt);
	else
		ret = redirection_wrap_builtins(data, cmdt, redirt);
	return (ret);
}
