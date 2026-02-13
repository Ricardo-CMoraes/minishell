/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:38:02 by jnovais           #+#    #+#             */
/*   Updated: 2026/01/23 23:38:02 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_cmd(t_cmd *cmd, char **envp)
{
    pid_t	pid;
    int		status;
    char	*path;

    path = find_cmd_path(cmd->args[0], envp);
    if (!path)
    {
        printf("minishell: %s: command not found\n", cmd->args[0]);
        return (127);
    }

	pid = fork();
	if (pid == -1)
    {
        perror("fork");
        free(path);
        return (1);
    }
	
	if (pid == 0)	
	{
		execve(path, cmd->args, envp);
		perror("execve");
		free(path);
		exit(127);
	}

	free(path);
	waitpid(pid, &status, 0);

	 if (WIFEXITED(status))
        return (WEXITSTATUS(status));

	return (1);
}
