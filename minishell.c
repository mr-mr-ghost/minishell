/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:48:59 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/13 17:28:18 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**read_and_split(char **cmd)
{
	char	*line;

	line = readline("Minishell> ");
	if (!line || line[0] == '\0') //bad? bash saves history of such line: ""
		return (NULL);
	add_history(line);
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

void	child_process(char **args, char **envp)
{
	if (!handle_redirection(args)) // TODO: adjust redirs for builtins
			exit(EXIT_FAILURE);
	if (ft_memcmp(args[0], "echo", ft_strlen("echo") + 1) == 0)
		echo_command(args); // TODO
	else if (ft_memcmp(args[0], "cd", ft_strlen("cd") + 1) == 0)
		cd_command(args); // TODO
	else if (ft_memcmp(args[0], "pwd", ft_strlen("pwd") + 1) == 0)
		pwd_command();
	else if (ft_memcmp(args[0], "export", ft_strlen("export") + 1) == 0)
		export_command(args); // TODO
	else if (ft_memcmp(args[0], "unset", ft_strlen("unset") + 1) == 0)
		unset_command(args); // TODO
	else if (ft_memcmp(args[0], "env", ft_strlen("env") + 1) == 0)
		env_command(envp); // TODO
	else
	{
		if (execve(args[0], args, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}

// Function to execute commands (edited chatgpt example)
int	execute_command(char **args, char **envp)
{
	pid_t	pid;
	int		status;

	if (args == NULL || args[0] == NULL)
		return (1);
	if (ft_memcmp(args[0], "exit", ft_strlen("exit") + 1) == 0)
		return (0);
	pid = fork();
	if (pid == 0)
		child_process(args, envp);
	else if (pid < 0)
		perror("fork");
	else // parent process
	{ //WUNTRACED vs 0 ?
		if (waitpid(pid, &status, WUNTRACED) == -1)
			perror("waitpid");
	}
	return (1);
}

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		rl_clear_history(); // might need to clear cmd too somehow
		exit(EXIT_SUCCESS);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	**cmd;
	int		status;

	if (argc > 1)
		return (EXIT_SUCCESS);
	(void)argv;
	(void)envp;
	status = 1;
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
		perror("setup handle_sigint()");
	while (status)
	{
		cmd = NULL;
		cmd = read_and_split(cmd);
		status = execute_command(cmd, envp);
		free_cmd(cmd);
	}
	rl_clear_history();
	return (EXIT_SUCCESS);
}
