/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/13 11:38:34 by gklimasa         ###   ########.fr       */
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

// Function to execute commands (edited chatgpt example)
int	execute_command(char **args, char **envp)
{
	pid_t	pid;
	int		status;

	(void)envp; // delete this when changing to execve
	if (args == NULL || args[0] == NULL)
		return (1);
	if (ft_memcmp(args[0], "exit", ft_strlen("exit") + 1) == 0)
		return (0);
	pid = fork();
	if (pid == 0) // child process
	{
		if (!handle_redirection(args))
			exit(EXIT_FAILURE);
		if (execvp(args[0], args) == -1) // add envp to execve
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
		perror("fork");
	else // parent process
	{ //WUNTRACED to check if the child process is stopped
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid");
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	**cmd;
	int		status;

	if (argc > 1)
		return (0);
	(void)argv;
	printf("%s\n\n", getenv("PATH"));
	status = 1;
	while (status)
	{
		cmd = NULL;
		cmd = read_and_split(cmd);
		status = execute_command(cmd, envp);
		free_cmd(cmd);
	}
	return (0);
}
