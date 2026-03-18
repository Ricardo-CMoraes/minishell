/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:38:02 by jnovais           #+#    #+#             */
/*   Updated: 2026/03/17 02:49:57 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redirections(t_cmd *cmd)
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

void	child_process(t_cmd *cmd, t_cmd *cmds, char ***envp)
{
	char	*path;

	close_other_fds(cmds, cmd);
	if (cmd->invalid)
	{
		close_and_reset_fds(cmd);
		exit(1);
	}
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
	handle_execve_error(cmd->args[0], path, 126);
}

static void	err_exit(char *cmd, char *msg, char *path, int status)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(msg, 2);
	free(path);
	exit(status);
}

void	handle_execve_error(char *cmd_name, char *path, int default_status)
{
	struct stat	path_stat;

	if (path && stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		err_exit(cmd_name, ": Is a directory\n", path, 126);
	if (errno == EACCES)
		err_exit(cmd_name, ": Permission denied\n", path, 126);
	if (errno == ENOENT)
		err_exit(cmd_name, ": No such file or directory\n", path, 127);
	perror("execve");
	free(path);
	exit(default_status);
}

pid_t	create_child_process(t_cmd *cmd, t_cmd *cmds, char ***envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		g_exit_status = 1;
		close_and_reset_fds(cmd);
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		child_process(cmd, cmds, envp);
	}
	return (pid);
}
