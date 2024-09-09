/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 10:34:26 by gklimasa          #+#    #+#             */
/*   Updated: 2024/08/21 17:16:54 by gklimasa         ###   ########.fr       */
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

char	*get_heredoc(char *delimiter)
{
	char	*heredoc;
	char	*hereline;

	ft_putstr_fd("> ", 1);
	heredoc = NULL;
	hereline = get_next_line(0);
	while (hereline)
	{
		if (ft_strncmp(hereline, delimiter, ft_strlen(delimiter)) == 0
			&& hereline[ft_strlen(delimiter)] == '\n')
			break;
		else
			ft_putstr_fd("> ", 1);
		heredoc = join_strings(heredoc, hereline);
		hereline = get_next_line(0);
	}
	if (hereline)
		free(hereline);
	return (heredoc);
}

// example of << delimiter: "cat <<'X' > t.txt		contentbla X"
void	handle_heredoc(t_data *data, t_token *cmdt, t_token *redirt)
{
	char	*heredoc;
	t_token	*secondredir;
	int		pipefd[2];
	pid_t	pid;

	(void)data;
	secondredir = return_redirt(redirt->next);
	heredoc = get_heredoc(redirt->next->value);
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return ;
	}
	pid = fork();
	g_sig.in_cmd = true;
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		perror("fork failed");
		return ;
	}
	if (pid == 0)
	{
		close(pipefd[1]); // Close the write end of the pipe
		dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to read from the pipe
		close(pipefd[0]); // Close the original pipe read descriptor
		child_process(data, cmdt, secondredir);
		perror("execve fail");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipefd[0]); // Close the read end of the pipe
		write(pipefd[1], heredoc, ft_strlen(heredoc));
		if (heredoc)
			free(heredoc);
		close(pipefd[1]); // send EOF to the child process
		wait(NULL);
	}
}

// wrapper function for redirection of builtin commands
int	redirection_wrap_builtins(t_data *data, t_token *cmdt, t_token *redir)
{
	int	status;
	int	minilib_stdout;

	minilib_stdout = dup(STDOUT_FILENO);
	if (minilib_stdout < 0)
	{
		perror("dup");
		return (1);
	}
	if (handle_redirection(redir->next, redir->type) == -1)
	{
		if (dup2(minilib_stdout, STDOUT_FILENO) < 0)
			perror("dup2");
		close(minilib_stdout);
		return (1);
	}
	status = check_launch_builtins(data, cmdt);
	if (dup2(minilib_stdout, STDOUT_FILENO) < 0)
		perror("dup2");
	close(minilib_stdout);
	return (status);
}

// handles input/output redirection (>, >>, <)
// opens a file and sets it as the new stdout or stdin
// returns 0 on success, -1 on failure of open or dup2
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
	{
		perror("open");
		return (-1);
	}
	if (type == TRUNC || type == APPEND)
		dupstatus = dup2(fd, STDOUT_FILENO);
	else
		dupstatus = dup2(fd, STDIN_FILENO);
	close(fd);
	if (dupstatus < 0)
	{
		perror("dup2");
		return (-1);
	}
	return (0);
}
