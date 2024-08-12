/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:45:55 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/12 08:45:35 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* INCLUDES ***************************************************************** */
# include <stdio.h>		// printf, perror
# include <stdlib.h>	// malloc, free, exit
// read, write, access, open, close, fork, getcwd, chdir, dup, dup2, pipe,
# include <unistd.h>	// isatty, ttyname, ttyslot, tcsetattr, tcgetattr
# include <fcntl.h>		// open, unlink
// fork, wait, waitpid, wait3, wait4, stat, lstat, fstat, opendir, readdir,
# include <sys/types.h>	// closedir
# include <sys/stat.h>	// stat, lstat, fstat
# include <dirent.h>	// opendir, readdir, closedir
# include <signal.h>	// signal, sigaction, sigemptyset, sigaddset, kill
# include <string.h>	// strerror
# include <errno.h>		// errno, perror
# include <sys/ioctl.h>	// ioctl
# include <curses.h>	// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <term.h>		// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <termios.h>	// tcsetattr, tcgetattr
# include <sys/wait.h>	// wait, waitpid, wait3, wait4
// readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay,
# include <readline/readline.h>	// add_history
# include <readline/history.h>	// add_history
# include <sys/wait.h>	// wait, waitpid, wait3, wait4
# include "libft/libft.h"

/* DEFINES AND STRUCTS ****************************************************** */

/* FUNCTIONS **************************************************************** */
/* minishell.c ************************************************************** */
char	**read_and_split(char **cmd);

/* utils.c ****************************************************************** */
void	free_cmd(char **cmd);

#endif
