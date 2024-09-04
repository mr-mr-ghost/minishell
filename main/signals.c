/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhoddy <jhoddy@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 14:38:49 by jhoddy            #+#    #+#             */
/*   Updated: 2024/08/15 14:38:49 by jhoddy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	signal_manager(void)
{
	struct sigaction	act;
	struct termios		term;

	tcgetattr(0, &term);
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(0, TCSANOW, &term);
	act.sa_handler = sigint_handler;
	act.sa_flags = SA_RESTART;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGINT);
	sigaction(SIGINT, &act, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_child_handler(int signum)
{
	(void)signum;
	exit(130);
}

void	sigint_handler(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (!g_sig.in_cmd)
		rl_redisplay();
	g_sig.sigint = 1;
}

void	sig_init(void)
{
	g_sig.in_cmd = false;
	g_sig.sigint = 0;
}
