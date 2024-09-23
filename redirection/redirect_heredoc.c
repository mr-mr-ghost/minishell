/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:37:15 by jhoddy            #+#    #+#             */
/*   Updated: 2024/09/22 14:37:15 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_strings(char *s1, char *s2)
{
	char	*joined;

	if (s1)
	{
		joined = ft_strjoin(s1, s2);
		free(s1);
	}
	else
		joined = ft_strdup(s2);
	free(s2);
	return (joined);
}

char	*heredoc_error(char *delimiter, char *heredoc)
{
	ft_putchar_fd('\n', 1);
	if (g_sig.sigint)
	{
		if (heredoc)
			free(heredoc);
		g_sig.sigint = 0;
		return (NULL);
	}
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
	return (heredoc);
}

char	*get_heredoc(char *delimiter)
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
		heredoc = join_strings(heredoc, hereline);
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

int	process_heredoc(t_data *data, t_token *cmdt, t_token *redir, int *fd)
{
	pid_t	pid;

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
		child_process(data, cmdt, redir);
	}
	return (0);
}

/* example of << delimiter: "cat <<'X' > t.txt		contentbla X"*/
int	handle_heredoc(t_data *data, t_token *cmdt, t_token *redirt)
{
	char	*heredoc;
	int		pipefd[2];
	t_token	*secondredir;

	secondredir = return_redirt(redirt->next);
	heredoc = get_heredoc(redirt->next->value);
	if (cmdt->next->type >= TRUNC && !heredoc)
		return (0);
	if (pipe(pipefd) == -1)
		return (err_msg(NULL, NULL, strerror(errno), 1));
	if (process_heredoc(data, cmdt, secondredir, pipefd))
		return (1);
	close(pipefd[0]);
	ft_putstr_fd(heredoc, pipefd[1]);
	if (heredoc)
		free(heredoc);
	close(pipefd[1]);
	wait(NULL);
	return (0);
}
