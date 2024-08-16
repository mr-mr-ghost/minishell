/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:45:55 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/16 12:28:31 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
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

# define EMPTY 0	// ""
# define CMD 1		// command
# define ARG 2		// option or argument
# define TRUNC 3	// >
# define APPEND 4	// >>
# define INPUT 5	// <
# define PIPE 6		// |
# define END 7		// ;

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
	char			*value;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_sig
{
	int		sigint;
	int		sigquit;
	int		exit_status;
}	t_sig;

typedef struct s_data
{
	char	*line;
	int		exit;
	t_env	*env;
	t_token	*token;
}	t_data;

/*	utils	*/
void	free_tokens(t_data *data);
void	free_env(t_env *env);
int		ft_strcmp(const char *s1, const char *s2);

/*	environment initialisation	*/
void	init_env(t_data *data, char **envp);
void	set_shell_lvl(t_env *env);

/*	environment utils	*/
char	*find_env_value(t_env *env, char *name);
int		find_env_lvl(char *lvl);
char	*get_env_name(char *dest, char *src);

/*	tokens handling	*/
void	token_split(t_data *data);
void	tokens_type_define(t_data *data);

/*	tokens split utils	*/
void	handle_special_chars(t_data *data, char *line, int *i);
void	handle_quotes(t_data *data, char *line, int *i);
void	handle_normal_chars(t_data *data, char *line, int *i, bool *check);
bool	handle_cmd(t_data *data, char *line, int *i);
char	*remove_quotes(char *str);

/*	tokens utils	*/
t_token	*token_new(char *value);
void	token_add_back(t_token **token, t_token *new);
int		is_cmd(char *line, int i);
bool	select_cmp(char *line, char *cmp, int start, int len);
bool	quotes_check(char *line, int i);

/*	signals	*/
void	sigint_handler(int signum);
void	sigquit_handler(int signum);
void	disable_sigquit(void);
void	sig_init(void);

/*	exec utils	*/
void	free_cmd(char **cmd);
int		process_n_exec(t_data *data, char **envp);

/*	redirection utils	*/
int		handle_redirection(char **args);

/*	builtin commands	*/
int		echo_command(char **args);
int		cd_command(char **args);
int		pwd_command(void);
int		export_command(char **args);
int		unset_command(char **args);
int		env_command(char **args, char **envp);
int		exit_command(t_data *data, char **args);

/*	global	*/
extern t_sig	g_sig;

#endif
