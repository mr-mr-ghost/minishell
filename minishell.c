/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/12 16:39:18 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**read_and_split(char **cmd)
{
	char	*line;

	line = readline("Minishell> ");
	cmd = ft_split(line, ' ');
	if (cmd == NULL || cmd[0] == NULL)
	{
		if (line)
			free(line);
		return (NULL);
	}
	free(line);
	return (cmd);
}

// Function to handle redirection (edited chatgpt example)
void	handle_redirection(char **args)
{
	int	i;
	int	fd;

	i = 0;
	fd = -1;
	while (args && args[i] != NULL)
	{
		if (ft_memcmp(args[i], ">", 2) == 0 || ft_memcmp(args[i], "<", 2) == 0)
		{
			if (args[i][0] == '>')
				fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (args[i][0] == '<')
				fd = open(args[i + 1], O_RDONLY);
			if (fd < 0)
			{
				perror("open");	// replace by free cmd and return empty line
				exit(EXIT_FAILURE);
			}
			if (args[i][0] == '>')
				dup2(fd, STDOUT_FILENO);
			else if (args[i][0] == '<')
				dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL; // not sure if needed
			break ;
		}
		i++;
	}
}

// Function to execute commands (chat gpt example)
int	execute_command(char **args)
{
	pid_t	pid;
	int		status;

	if (args == NULL || args[0] == NULL)
		return 1;
	if (ft_memcmp(args[0], "exit", ft_strlen("exit") + 1) == 0)
		return 0;
	pid = fork();
	if (pid == 0)
	{ // Child process
		handle_redirection(args);
		if (execvp(args[0], args) == -1) // we need to use execve instead
			perror("execvp");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("fork");
	else
	{ // replace this whole part with allowed functions
		do // Parent process
			waitpid(pid, &status, WUNTRACED);
		while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

int	main(void)
{
	char		**cmd;
	int			status;
	const char	*path;

	path = getenv("PATH");
	printf("%s\n", path);
	status = 1;
	while (status)
	{
		cmd = NULL;
		cmd = read_and_split(cmd);
		status = execute_command(cmd);
		free_cmd(cmd);
	}
	return (0);
}
