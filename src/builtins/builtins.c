/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:00:00 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/04 20:50:00 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "echo", 5) == 0
		|| ft_strncmp(cmd, "env", 4) == 0
		|| ft_strncmp(cmd, "cd", 3) == 0
		|| ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0
		|| ft_strncmp(cmd, "exit", 5) == 0);
}

int	execute_builtin(t_cmd *cmd, char ***envp)
{
	int	fd_out;

	if (!cmd || !cmd->args || !cmd->args[0] || !envp)
		return (0);
	fd_out = cmd->fd_out;
	if (fd_out < 0)
		return (1);
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (fd_pwd(fd_out));
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (fd_echo(cmd->args, fd_out));
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (fd_env(*envp, fd_out));
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (fd_cd(cmd->args, envp, fd_out));
	if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (fd_export(cmd->args, envp, fd_out));
	if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (fd_unset(cmd->args, envp));
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (fd_exit(cmd->args));
	return (0);
}
