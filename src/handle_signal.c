
#include "minishell.h"

void handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

void handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_exit_status = 130;
	printf("\n");
	close(0);
}

void setup_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}