/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:45:55 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/04 13:22:47 by gklimasa         ###   ########.fr       */
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

# define EMPTY 0
# define CMD 1
# define ARG 2
# define TRUNC 3
# define APPEND 4
# define INPUT 5
# define HEREDOC 6
# define PIPE 7
# define END 8

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
	bool			div;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_sig
{
	bool	in_cmd;
	int		sigint;
}	t_sig;

typedef struct s_data
{
	char	*line;
	bool	end;
	int		exit_code;
	t_env	*env;
	t_env	*secret_env;
	t_token	*token;
}	t_data;

/*	prompt	*/
char	*read_line(t_env *env);
char	*generate_prompt(t_env *env);
char	*shorten_path(char *path);

/*	utils	*/
void	free_tokens(t_data *data);
void	free_env(t_env *env);
bool	check_char(char *line);
int		err_msg(char *location, char *arg, char *msg, int code);

/*	libft utils	*/
int		ft_strcmp(const char *s1, const char *s2);

/*	environment initialisation	*/
void	init_env(t_data *data, char **envp);
t_env	*create_env_list(t_env *env, char **envp);
void	set_shell_lvl(t_env *env);
char	*set_env_name(char *line);
char	*set_env_value(char *line);

/*	environment utils	*/
char	*find_env_line(t_env *env, char *key);
int		find_env_lvl(char *lvl);
char	*get_env_name(char *dest, const char *src);
char	*find_env_value(t_env *env, char *key);
char	*find_env_name(t_env *env, char *key);

/*	environment list utils	*/
t_env	*env_new(char *value);
void	env_add_back(t_env **env, char *value);
void	env_delone(t_env *env);
void	env_replace(t_env *env, char *name, char *line);

/*	tokens handling	*/
int		token_split(t_data *data);
void	tokens_type_define(t_data *data);
int		process_token(t_data *data);

/*	tokens split utils	*/
int		handle_special_chars(t_data *data, char *line, int *i);
void	handle_quotes(t_data *data, char *buffer, int *i, int *k);
int		handle_normal_chars(t_data *data, int *i);

/*	tokens split special	*/
char	*process_dollar(t_data *data, char *line, int *i);
void	add_dollar_value(t_data *data, char *buffer, int *j, int *k);
char	*get_dollar_value(t_env *env, char *line, int *start);

/*	tokens list utils	*/
t_token	*token_new(char *value, bool div);
void	token_add_back(t_token **token, t_token *new);

/*	tokens utils	*/
bool	select_cmp(const char *line, const char *cmp, int start, int len);
bool	quotes_check(char *line);
int		token_err(t_data *data, char *arg, char *msg, int code);

/*	signals	*/
void	sigint_handler(int signum);
void	sig_init(void);
void	signal_manager(void (*handler)(int), int flag);
void	sig_child_handler(int signum);
void	heredoc_sig_handler(int signum);

/*	pwd command	*/
int		pwd_command(void);

/*	exit command	*/
int		validate_exit(char *arg);
int		exit_command(t_data *data, t_token *token);

/*	export command	*/
int		export_command(t_data *data, t_token *token);
int		process_export(t_data *data, t_token *export_token);
int		single_export(t_env *env);
int		handle_assign(t_data *data, t_token *token);

/*	env command	*/
int		env_command(t_data *data, t_token *token);

/*	echo command	*/
void	print_echo(t_token *echo_token);
int		echo_command(t_token *token);

/*	unset command	*/
int		unset_command(t_data *data, t_token *token);
void	del_env(t_env **env, char *key);

/*	cd command	*/
int		cd_command(t_data *data, t_token *token);
char	*determine_path(t_env *env, t_token *token);
int		change_env_path(t_data *data, char *old_pwd);
int		replace_path(t_env *env, char *name, char *path);
char	*set_back_dir(void);

/*	declaration handling	*/
int		handle_declaration(t_env *secret_env, t_token *token);
char	*check_declaration(t_token *token);

/*	builtins utils	*/
bool	valid_env_name(t_env *env, char *key);
char	*add_quotes_var(char *line);
bool	select_valid_env(t_env *env, char *line, int start);
bool	check_numeric(char *str);

/*	non-builtins handling	*/
int		launch_nonbuiltins(t_data *data, t_token *cmd, t_token *redirt);

/*	non-builtins utils	*/
char	*find_bin(t_env *env, char *cmd);
char	**bin_path(t_env *env);
char	*check_dir(char *bin, char *cmd);
char	*join_path(char *path, char *cmd);

/*	non-builtins processes	*/
void	child_process(t_data *data, t_token *cmdt, t_token *redirt);
int		check_error(char *path);
void	bin_error(t_data *data, char *cmd);

/*	execution	*/
void	process_n_exec(t_data *data);

/*	execution utils	*/
int		check_launch_builtins(t_data *data, t_token *token);
int		is_cmd(char *line, int i);

/*	execution array utils	*/
char	**form_enva(t_env *env);
char	**form_cmd(t_token *cmd, int size);
int		count_args(t_token *cmd, int type);
void	free_array(char **array);

/*	redirections	*/
int		handle_redirection(t_token *fname, int type);
int		redirection_wrap_builtins(t_data *data, t_token *cmdt, t_token *redir);

/*	heredoc redirect*/
char	*get_heredoc(t_data *data, char *delimiter);
int		process_heredoc(t_data *data, t_token *cmdt, int *fd, char *heredoc);
int		handle_heredoc(t_data *data, t_token *cmdt, t_token *hdtoken);
int		handle_heredoc_builtins(t_data *data, t_token *cmdt, t_token *hdtoken);

/*	heredoc redirect utils	*/
char	*heredoc_error(char *delimiter, char *heredoc);
char	*join_strings(t_data *data, char *s1, char *s2);
int		heredoc_strcat(char *dest, char *src, int start);

t_token	*get_nth_token(t_token *token, int n);
t_token	*return_heredoct(t_token *cmdt);
t_token	*return_redirt(t_token *cmdt);
int		launch_single_anycmd(t_data *data, t_token *cmdt);

/*	pipes	*/
int		launch_cmd_inpipe(t_data *data, t_token *cmdt);
int		pipe_fork(t_data *data, t_token *cmdt, int *input_fd, int *output_fd);
int		call_pipe(t_data *data, t_token *currentt);

/*	global	*/
extern t_sig	g_sig;

#endif
