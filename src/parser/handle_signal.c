/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 17:33:03 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/04 00:39:40 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(1, "\n", 1);
	close(0);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	write(2, "Quit (core dumped)\n", 19);
	g_exit_status = 131;
	exit(131);
}

void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
