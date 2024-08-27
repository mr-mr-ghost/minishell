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

#include "minishell.h"

void	signal_manager(void)
{
	struct sigaction	act;
	struct termios		term;

	tcgetattr(0, &term);
	term.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(0, TCSANOW, &term);
	act.sa_handler = signal_handler;
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}

void	signal_handler(int signum)
{
	if (signum == SIGINT)
		sigint_handler(signum);
	else if (signum == SIGQUIT)
		sigquit_handler(signum);
}

void	sigint_handler(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	rl_clear_history();
	g_sig.sigint = 1;
	g_sig.exit_status = 130;
}

void	sigquit_handler(int signum)
{
	(void)signum;
	g_sig.sigquit = 1;
}

void	sig_init(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.exit_status = 0;
}
