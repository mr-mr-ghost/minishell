/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nonbuiltin_processes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:53:19 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/18 14:42:52 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (fd >= 0)
		close(fd);
	return (exit_code);
}

void	bin_error(t_data *data, char *cmd)
{
	int	exit_status;

	exit_status = check_error(cmd);
	free_tokens(data);
	free_env(data->env);
	free_env(data->secret_env);
	exit(exit_status);
}

void	child_cleanexit(t_data *data, char *bin, char **enva, char **cmda)
{
	int	exit_status;

	if (bin)
	{
		exit_status = check_error(bin);
		free(bin);
	}
	else
		exit_status = 1;
	if (enva)
		free(enva);
	if (cmda)
		free(cmda);
	rl_clear_history();
	free_tokens(data);
	free_env(data->env);
	free_env(data->secret_env);
	exit(exit_status);
}

int	check_launch_redir(t_token *redirt)
{
	while (redirt)
	{
		if (redirt->type >= TRUNC && redirt->type <= INPUT
			&& handle_redirection(redirt->next, redirt->type) != 0)
			return (1);
		redirt = return_redirt(redirt->next);
	}
	return (0);
}

void	child_process(t_data *data, t_token *cmdt, t_token *redirt)
{
	char	**enva;
	char	**cmda;
	char	*bin;

	bin = NULL;
	enva = NULL;
	cmda = NULL;
	signal_manager(sig_child_handler, 0);
	if (check_launch_redir(redirt) == 1)
		child_cleanexit(data, bin, enva, cmda);
	bin = find_bin(data->env, cmdt->value);
	if (!bin)
		bin_error(data, cmdt->value);
	enva = form_enva(data->env, data->secret_env);
	if (!enva)
		child_cleanexit(data, bin, enva, cmda);
	cmda = form_cmd(data, cmdt, count_args(cmdt, TRUNC));
	if (!cmda)
		child_cleanexit(data, bin, enva, cmda);
	execve(bin, cmda, enva);
	child_cleanexit(data, bin, enva, cmda);
}
