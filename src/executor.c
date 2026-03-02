/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:38:02 by jnovais           #+#    #+#             */
/*   Updated: 2026/02/12 01:14:00 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	apply_redirections(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		close(cmd->fd_in);
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(cmd->fd_out);
	}
}

static void	close_other_fds(t_cmd *cmds, t_cmd *current)
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

static void	child_process(t_cmd *cmd, t_cmd *cmds, char ***envp)
{
	char	*path;

	close_other_fds(cmds, cmd);
	apply_redirections(cmd);
	if (is_builtin(cmd->args[0]))
	{
		cmd->fd_out = STDOUT_FILENO;
		cmd->fd_in = STDIN_FILENO;
		exit(execute_builtin(cmd, envp));
	}
	path = find_cmd_path(cmd->args[0], *envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(path, cmd->args, *envp);
	perror("execve");
	free(path);
	exit(126);
}

static void	close_and_reset_fds(t_cmd *cmd)
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

int	execute_cmd(t_cmd *cmd, char **envp)
{
	pid_t	pid;
	int		status;
	char	*path;

	if (!cmd || cmd->invalid || !cmd->args || !cmd->args[0])
	{
		if (cmd)
			close_and_reset_fds(cmd);
		return (g_exit_status);
	}
	path = find_cmd_path(cmd->args[0], envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
		return (127);
	}
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
		perror("execve");
		free(path);
		exit(127);
	}
	free(path);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
	else
		g_exit_status = 1;
	return (g_exit_status);
}

void	execute_pipeline(t_cmd *cmds, char ***envp)
{
	t_cmd	*cmd;
	pid_t	last_pid;
	int		status;
	int		executed_any;
	int		had_invalid;

	cmd = cmds;
	last_pid = -1;
	signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
	executed_any = 0;
	had_invalid = 0;
	while (cmd)
	{
		if (cmd->invalid || !cmd->args || !cmd->args[0])
		{
			close_and_reset_fds(cmd);
			if (cmd->invalid)
				had_invalid = 1;
			cmd = cmd->next;
			continue ;
		}
		if (!cmds->next && is_builtin(cmd->args[0]))
		{
			g_exit_status = execute_builtin(cmd, envp);
			executed_any = 1;
			close_and_reset_fds(cmd);
			cmd = cmd->next;
			continue ;
		}
		last_pid = fork();
		if (last_pid == -1)
		{
			perror("fork");
			g_exit_status = 1;
			close_and_reset_fds(cmd);
			cmd = cmd->next;
			continue ;
		}
		executed_any = 1;
		if (last_pid == 0)
		{
			signal(SIGINT, SIG_DFL);
    		signal(SIGQUIT, SIG_DFL);
			child_process(cmd, cmds, envp);
		}
		close_and_reset_fds(cmd);
		cmd = cmd->next;
	}
	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			g_exit_status = 128 + WTERMSIG(status);
			if (g_exit_status == 130)
                write(1, "\n", 1);
		}
	}
	else if (!executed_any)
	{
		if (had_invalid && g_exit_status == 0)
			g_exit_status = 1;
		else if (!had_invalid)
			g_exit_status = 0;
	}
	while (wait(NULL) > 0)
		;
	setup_signals();
}
