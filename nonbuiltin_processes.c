/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nonbuiltin_processes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:53:19 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/26 15:53:19 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_error(char *path)
{
	DIR	*dir;
	int	fd;
	int	exit_code;

	fd = open(path, O_RDONLY);
	dir = opendir(path);
	exit_code = 0;
	if (!ft_strchr(path, '/'))
		exit_code = err_msg(NULL, path, "Command not found", 127);
	else if (fd == -1 && dir == NULL)
		exit_code = err_msg(NULL, path, "No such file or directory", 127);
	else if (fd == -1 && dir != NULL)
		exit_code = err_msg(NULL, path, "Is a directory", 126);
	else if (fd != -1 && dir == NULL)
		exit_code = err_msg(NULL, path, "Permission denied", 126);
	if (dir)
		closedir(dir);
	close(fd);
	return (exit_code);
}

void	child_process(t_data *data, t_token *cmdt, t_token *redirt, char *path)
{
	char	**enva;
	char	**cmda;
	int		exit_code;

//	printf("Inside child process with PID: %d\n", getpid());
	enva = form_enva(data->env);
	if (!enva)
		exit(EXIT_FAILURE);
	cmda = form_cmd(cmdt, count_args(cmdt, TRUNC));
	if (!cmda)
	{
		free(enva);
		exit(EXIT_FAILURE);
	}
	if (redirt && handle_redirection(redirt->next, redirt->type) == -1)
	{
		free(cmda);
		free(enva);
		exit(EXIT_FAILURE);
	}
	execve(path, cmda, enva);
	exit_code = check_error(path);
	free(cmda);
	free(enva);
	free_tokens(data);
	exit(exit_code);
}
