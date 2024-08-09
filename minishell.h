/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:45:55 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/09 16:51:55 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
TODO: Allow repeated input
TODO: Split input into tokens
TODO: Write Commands
TODO: Piping/flags/etc...
TODO: Parsing
TODO: Fix misc. errors
LETS DO THIS!!!
*/

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

char	**read_and_split(char **cmd);
void	free_cmd(char **cmd);

#endif
