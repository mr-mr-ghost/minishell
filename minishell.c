/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/12 22:54:12 by gklimasa         ###   ########.fr       */
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

void	delete_array_element(char **array, int index)
{
	int	i;

	i = index;
	while (array[i] != NULL)
	{
		array[i] = array[i + 1];
		i++;
	}
}

// Function to handle redirection (edited chatgpt example)
int	handle_redirection(char **args)
{
	int	i;
	int	fd;

	i = 0;
	while (args[i] != NULL &&
		ft_memcmp(args[i], ">", 2) != 0 && ft_memcmp(args[i], "<", 2) != 0)
		i++;
	if (args[i] == NULL)
		return (1);
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
	delete_array_element(args, i);
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
	{ // child process
		if (!handle_redirection(args))
			exit(EXIT_FAILURE);
		if (execvp(args[0], args) == -1) // need execve instead
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
		perror("fork");
	else
	{ // parent process
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid");
	}
	return (1);
}

int	main(void)
{
	char		**cmd;
	int			status;
	const char	*path;

	path = getenv("PATH");
	printf("%s\n\n", path);
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
