/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nonbuiltin_processes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 15:53:19 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/19 21:06:51 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_error(char *path)
{
	DIR	*dir;
	int	fd;
	int	exit_code;

	fd = open(path, O_WRONLY);
	dir = opendir(path);
	exit_code = 0;
	if (!ft_strchr(path, '/'))
		exit_code = err_msg(NULL, path, "Command not found", 127);
	else if (access(path, F_OK))
		exit_code = err_msg(NULL, path, strerror(errno), 127);
	else if (fd == -1 && dir != NULL)
		exit_code = err_msg(NULL, path, "Is a directory", 126);
	else if (access(path, X_OK))
		exit_code = err_msg(NULL, path, strerror(errno), 126);
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
	rl_clear_history();
	free_tokens(data);
	free_env(data->env);
	free_env(data->secret_env);
	exit(exit_status);
}

// exits the child proccess and frees all it's allocated memory
void	child_cleanexit(t_data *data, char *bin, char **enva, char **cmda)
{
	int	exit_status;

	exit_status = 1;
	if (bin)
	{
		exit_status = check_error(bin);
		free(bin);
	}
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

// launches the redirections (< > >>)
// deletes and skips the input redirection if the next next token is an argument
// returns 0 on success or 1 on error
int	check_launch_redir(t_token *redirt)
{
	while (redirt)
	{
		if ((redirt->type == TRUNC || redirt->type == APPEND)
			&& handle_redirection(redirt->next, redirt->type) != 0)
			return (1);
		else if (redirt->type == INPUT)
		{
			if (redirt->next && redirt->next->type == CMD
				&& redirt->next->next && redirt->next->next->type == ARG)
			{
				redirt = delete_token(redirt);
				redirt = delete_token(redirt);
				continue ;
			}
			else
			{
				if (handle_redirection(redirt->next, redirt->type) != 0)
					return (1);
			}
		}
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
	enva = form_enva(data->env);
	cmda = form_cmd(cmdt, count_args(cmdt, TRUNC));
	if (!enva || !cmda)
		child_cleanexit(data, bin, enva, cmda);
	execve(bin, cmda, enva);
	child_cleanexit(data, bin, enva, cmda);
}
