/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:37:15 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/08 12:45:08 by jhoddy           ###   ########.fr       */
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
	t_token	*redirt;

	redirt = return_redirt(cmdt);
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
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (heredoc)
			free(heredoc);
		child_process(data, cmdt, redirt);
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
	if (g_sigint) // ctrl + c
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

int	handle_heredoc_builtins(t_data *data, t_token *cmdt, t_token *hdtoken)
{
	char	*heredoc;
	t_token	*redirt;
	int		ret;

	heredoc = get_heredoc(data, hdtoken->next->value);
	if (g_sigint) // ctrl + c
		return (130);
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
