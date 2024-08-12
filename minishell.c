/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/12 18:11:21 by gklimasa         ###   ########.fr       */
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
int	handle_redirection(char **args)
{
	int	i;
	int	fd;

	i = 0;
	while (args[i] != NULL)
	{
		if (ft_memcmp(args[i], ">", 2) == 0 || ft_memcmp(args[i], "<", 2) == 0)
		{
			if (args[i][0] == '>')
				fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fd = open(args[i + 1], O_RDONLY);
			if (fd < 0)
			{
				perror("open");
				return (0);
			}
			if (args[i][0] == '>')
				dup2(fd, STDOUT_FILENO);
			else
				dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL; // not sure if needed
			return (1);
		}
		i++;
	}
	return (1);
}

// Function to execute commands (edited chatgpt example)
int	execute_command(char **args)
{
	pid_t	pid;
	int		status;

	if (args == NULL || args[0] == NULL)
		return (1);
	if (ft_memcmp(args[0], "exit", ft_strlen("exit") + 1) == 0)
		return (0);
	pid = fork();
	if (pid == 0)
	{ // Child process
		if (handle_redirection(args) == 0)
			return (1); // not working, because parent process?
		if (execvp(args[0], args) == -1) // we need to use execve instead
			perror("execvp");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		perror("fork");
	else
	{
		if (waitpid(pid, &status, 0) == -1) //WUNTRACED
			perror("waitpid");
		//while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return (1);
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
