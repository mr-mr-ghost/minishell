/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gklimasa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 16:45:55 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/14 11:56:19 by gklimasa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* INCLUDES ***************************************************************** */
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

/* DESCRIPTION OF ALLOWED FUNCTIONS ***************************************** */
/*
readline/readline.h
	1 readline: Reads a line of text from the user with editing and history capabilities.
	2 rl_clear_history: Clears the readline command history.
	3 rl_on_new_line: Updates internal data structures to reflect that the cursor is on a new line.
	4 rl_replace_line: Replaces the current line in the readline buffer with a new one.
	5 rl_redisplay: Redisplays the current contents of the readline buffer.
readline/history.h
	6 add_history: Adds a line of text to the readline history.
stdio.h
	7 printf: Formats and prints a string to the standard output.
	8 perror: Prints a descriptive error message to standard error based on the current errno.
string.h
	9 strerror: Returns a string describing the error code.
stdlib.h
	10 malloc: Allocates a specified amount of memory on the heap.
	11 free: Frees previously allocated memory to prevent memory leaks.
	12 exit: Terminates the calling process and returns a status code to the operating system.
	13 getenv: Retrieves the value of an environment variable.
unistd.h
	14 write: Writes data to a file descriptor.
	15 access: Checks a file's accessibility based on the specified mode (read, write, execute).
	16 read: Reads data from a file descriptor into a buffer.
	17 close: Closes a file descriptor, releasing the associated resources.
	18 fork: Creates a new process by duplicating the calling process.
	19 getcwd: Retrieves the current working directory.
	20 chdir: Changes the current working directory to the specified path.
	21 unlink: Deletes a name from the filesystem, effectively removing a file.
	22 execve: Replaces the current process image with a new process image.
	23 dup: Duplicates a file descriptor.
	24 dup2: Duplicates a file descriptor to a specified file descriptor.
	25 pipe: Creates a unidirectional data channel (pipe) for interprocess communication.
	26 isatty: Checks if a file descriptor refers to a terminal.
	27 ttyname: Returns the name of the terminal associated with a file descriptor.
	28 ttyslot: Returns the index of the current terminal in the terminal list.
fcntl.h
	29 open: Opens a file and returns a file descriptor.
sys/wait.h
	30 wait: Suspends execution of the calling process until one of its children terminates.
	31 waitpid: Waits for a specific child process to terminate.
	32 wait3: Waits for a child process to change state, with resource usage statistics.
	33 wait4: Similar to wait3 but allows waiting for a specific process.
sys/stat.h
	39 stat: Retrieves information about a file based on its pathname.
	40 lstat: Similar to stat but does not follow symbolic links.
	41 fstat: Retrieves information about an open file based on its file descriptor.
sys/ioctl.h
	42 ioctl: Performs device-specific input/output operations on a file descriptor.
signal.h
	34 signal: Sets up a signal handler for a specific signal.
	35 sigaction: Examines or changes the action to be taken on receipt of a specific signal.
	36 sigemptyset: Initializes a signal set to exclude all signals.
	37 sigaddset: Adds a signal to a signal set.
	38 kill: Sends a signal to a process or group of processes.
dirent.h
	43 opendir: Opens a directory stream corresponding to a directory.
	44 readdir: Reads an entry from a directory stream.
	45 closedir: Closes a directory stream.
termios.h
	46 tcsetattr: Sets the parameters associated with the terminal.
	47 tcgetattr: Gets the parameters associated with the terminal.
curses.h or term.h
	48 tgetent: Loads a terminal entry from the termcap database.
	49 tgetflag: Gets the value of a boolean capability for the terminal.
	50 tgetnum: Gets the value of a numeric capability for the terminal.
	51 tgetstr: Retrieves a string capability from the termcap database.
	52 tgoto: Produces a cursor-movement string using a given capability.
	53 tputs: Outputs a terminal control string with padding.
 */

/* DEFINES ****************************************************************** */
# define MAX_COMMAND_LENGTH 1024
# define MAX_ARGS 64

/* STRUCTS ****************************************************************** */

/* FUNCTIONS **************************************************************** */
/* minishell.c ************************************************************** */
char	**read_and_split(char **cmd);

/* exec_utils.c ************************************************************* */
int		execute_command(char **args, char **envp);

/* redirection_utils.c ****************************************************** */
int		handle_redirection(char **args);

/* builtin_commands.c ******************************************************* */
void	echo_command(char **args);
void	cd_command(char **args);
void	pwd_command(void);
void	export_command(char **args);
void	unset_command(char **args);
void	env_command(char **envp);

/* utils.c ****************************************************************** */
void	free_cmd(char **cmd);

#endif
