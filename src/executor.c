/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:38:02 by jnovais           #+#    #+#             */
/*   Updated: 2026/03/17 01:50:09 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_exit_status(int status)
{
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
	else
		g_exit_status = 1;
	return (g_exit_status);
}

static void	wait_and_handle_status(pid_t last_pid, int executed_any,
		int had_invalid)
{
	int	status;

	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		handle_pipeline_status(status, executed_any, had_invalid);
	}
	else if (!executed_any)
	{
		if (had_invalid && g_exit_status == 0)
			g_exit_status = 1;
		else if (!had_invalid)
			g_exit_status = 0;
	}
}

static int	fork_and_execute(t_cmd *cmd, char *path, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		g_exit_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		apply_redirections(cmd);
		execve(path, cmd->args, envp);
		handle_execve_error(cmd->args[0], path, 126);
	}
	free(path);
	waitpid(pid, &status, 0);
	return (update_exit_status(status));
}

int	execute_cmd(t_cmd *cmd, char **envp)
{
	char	*path;

	if (!cmd || cmd->invalid || !cmd->args || !cmd->args[0])
	{
		if (cmd)
			close_and_reset_fds(cmd);
		return (g_exit_status);
	}
	path = find_cmd_path(cmd->args[0], envp);
	if (!path)
		return (handle_cmd_not_found(cmd));
	return (fork_and_execute(cmd, path, envp));
}

void	execute_pipeline(t_cmd *cmds, char ***envp)
{
	t_exec_ctx	ctx;
	t_cmd		*last;

	ctx.cmds = cmds;
	ctx.current = cmds;
	ctx.last_pid = -1;
	ctx.executed_any = 0;
	ctx.had_invalid = 0;
	ctx.envp = envp;
	setup_pipeline_signals();
	process_cmd_loop(&ctx);
	last = cmds;
	while (last && last->next)
		last = last->next;
	wait_and_handle_status(ctx.last_pid, ctx.executed_any, (last && last->invalid));
	restore_signals_and_wait();
}
