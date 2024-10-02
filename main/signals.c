/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:38:49 by jhoddy            #+#    #+#             */
/*   Updated: 2024/10/02 11:54:22 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signal_manager(void (*handler)(int), int flag)
{
	struct sigaction	act;
	struct termios		term;

	act.sa_handler = handler;
	act.sa_flags = flag;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGINT);
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
	if (tcgetattr(0, &term) == -1)
		return ;
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(0, TCSANOW, &term);
}

void	sig_child_handler(int signum)
{
	if (signum == SIGINT)
		exit(130);
}

void	sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sigint = 1;
	}
}

void	sigint_handler_incmd(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		g_sigint = 1;
	}
}

void	heredoc_sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_sigint = 1;
		rl_on_new_line();
	}
}
