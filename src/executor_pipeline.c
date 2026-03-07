/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 11:33:45 by jnovais           #+#    #+#             */
/*   Updated: 2026/03/03 11:33:45 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipeline_status(int status, int executed_any, int had_invalid)
{
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (g_exit_status == 130)
			write(1, "\n", 1);
	}
	else if (!executed_any)
	{
		if (had_invalid && g_exit_status == 0)
			g_exit_status = 1;
		else if (!had_invalid)
			g_exit_status = 0;
	}
}

void	close_and_reset_fds(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		close(cmd->fd_in);
		cmd->fd_in = STDIN_FILENO;
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		close(cmd->fd_out);
		cmd->fd_out = STDOUT_FILENO;
	}
}

void	close_other_fds(t_cmd *cmds, t_cmd *current)
{
	while (cmds)
	{
		if (cmds != current)
		{
			if (cmds->fd_in != STDIN_FILENO)
				close(cmds->fd_in);
			if (cmds->fd_out != STDOUT_FILENO)
				close(cmds->fd_out);
		}
		cmds = cmds->next;
	}
}
