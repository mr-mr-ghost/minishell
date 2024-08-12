/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:45:55 by jhoddy            #+#    #+#             */
/*   Updated: 2024/07/10 16:46:05 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include "libft/libft.h"

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define PIPE 6
# define END 7

# define MAX_ARGS 42
# define CMD_SIZE 1024
# define BUFF_SIZE 4096

typedef struct s_env
{
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	int				type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_data
{
	char	*line;
	char	**cmd;
	int		exit;
	t_env	*env;
	t_token	*token;
}	t_data;

char	**read_and_split(char *line);

/* utils */
void	free_tokens(t_token *token);
void	free_env(t_env *env);
char	*get_env_name(char *dest, char *src);
void	ft_memdel(void *ptr);

/* initialisation */
void	init_env(t_data *data, char **envp);
void	set_shell_lvl(t_env *env);

#endif
