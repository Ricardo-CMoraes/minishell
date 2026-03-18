/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:38:02 by jnovais           #+#    #+#             */
/*   Updated: 2026/03/17 01:59:29 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_cmd_not_found(t_cmd *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	g_exit_status = 127;
	return (127);
}

void	setup_pipeline_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_signals_and_wait(void)
{
	while (wait(NULL) > 0)
		;
	setup_signals();
}

static int	process_single_builtin(t_cmd *cmd, t_cmd *cmds, char ***envp)
{
	if (!cmds->next && is_builtin(cmd->args[0]))
	{
		g_exit_status = execute_builtin(cmd, envp);
		close_and_reset_fds(cmd);
		return (1);
	}
	return (0);
}

int	process_cmd_loop(t_exec_ctx *ctx)
{
	while (ctx->current)
	{
		if (ctx->current->invalid || !ctx->current->args
			|| !ctx->current->args[0])
		{
			close_and_reset_fds(ctx->current);
			ctx->current = ctx->current->next;
			continue ;
		}
		if (process_single_builtin(ctx->current, ctx->cmds, ctx->envp))
		{
			ctx->executed_any = 1;
			ctx->current = ctx->current->next;
			continue ;
		}
		ctx->last_pid = create_child_process(ctx->current, ctx->cmds,
				ctx->envp);
		if (ctx->last_pid > 0)
			ctx->executed_any = 1;
		close_and_reset_fds(ctx->current);
		ctx->current = ctx->current->next;
	}
	return (0);
}
