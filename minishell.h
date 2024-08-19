/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:45:55 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/18 11:49:57 by gklimasa         ###   ########.fr       */
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
# include <fcntl.h>
# include <errno.h>
# include <stdbool.h>
# include "libft/libft.h"

# define EMPTY 0	// ""
# define CMD 1		// command
# define ARG 2		// option or argument
# define TRUNC 3	// >
# define APPEND 4	// >>
# define INPUT 5	// <
# define PIPE 6		// |
# define HEREDOC 7	// <<
# define END 8		// ;

# define MAX_ARGS 42
# define CMD_SIZE 1024
# define BUFF_SIZE 4096

typedef struct s_env
{
	char			*line;
	char			*name;
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
	t_env	*secret_env;
	t_token	*token;
}	t_data;

/*	utils	*/
void	free_tokens(t_data *data);
void	free_env(t_env *env);
int		ft_strcmp(const char *s1, const char *s2);
char	*remove_quotes(char *line);

/*	environment initialisation	*/

int 	init_env(t_data *data, char **envp);
t_env	*create_env_list(t_env *env, char **envp);
void	set_shell_lvl(t_env *env);
char	*set_env_name(char *line);
char	*set_env_value(char *line);

/*	environment utils	*/
char	*find_env_line(t_env *env, char *key);
int		find_env_lvl(char *lvl);
char	*get_env_name(char *dest, char *src);
t_env	*env_new(char *value);

void	env_add_back(t_env **env, char *value);

/*	tokens handling	*/
void	token_split(t_data *data);
void	tokens_type_define(t_data *data);
void	process_token(t_data *data, char *line);
int		is_cmd(char *line, int i);

/*	tokens split utils	*/
void	handle_special_chars(t_data *data, char *line, int *i);
void	handle_quotes(t_data *data, char *line, int *i);
void	handle_normal_chars(t_data *data, char *line, int *i);
int 	handle_cmd(t_data *data, char *line, int *i);
void	handle_echo_chars(t_data *data, char *line, int *i);
void	handle_export_chars(t_data *data, char *line, int *i);

/*	tokens utils	*/
t_token	*token_new(char *value);
void	token_add_back(t_token **token, t_token *new);
bool	select_cmp(char *line, char *cmp, int start, int len);
bool	quotes_check(char *line, int i);
char	*token_remove_quotes(char *str);

/*	signals	*/
void	sigint_handler(int signum);
void	sigquit_handler(int signum);
void	disable_sigquit(void);
void	sig_init(void);

/*	builtins	*/
int		echo_command(t_token *token);
int		cd_command(t_token *token);
int		pwd_command(void);
int		unset_command(t_token *token);
int		exit_command(t_data *data, t_token *token);

/*	export command	*/
int 	export_command(t_data *data, t_token *token);
bool	valid_env_name(t_env *env, char *key);
int single_export(t_env *env);

/*	env command	*/
int		env_command(t_token *token, t_env *env);
int		print_env(t_env *env);

/*	execution	*/
int		process_n_exec(t_data *data, char **envp);
int		launch_nonbuiltins(char **cmd, char **envp, t_token *token);
int		check_launch_builtins(t_data *data, t_token *token, char **envp);
void	child_process(char **cmd, char **envp, t_token *token);
t_token	*get_nth_token(t_token *token, int n);

/*	redirections	*/
int		redirection_wrap_builtins(t_data *data, t_token *redir, char **envp);
int		handle_redirection(t_token *token, int type);

/*	commands array utils	*/
void	free_cmd(char **cmd);
int		count_args(t_token *token, int type);
char	**form_cmd(t_token *token, int len);

/*	global	*/
extern t_sig	g_sig;

/*	redirections	*/
int	handle_redirection(char **cmd);
void	delete_array_element(char **array, int index);

/*	global	*/
extern t_sig	g_sig;

#endif
