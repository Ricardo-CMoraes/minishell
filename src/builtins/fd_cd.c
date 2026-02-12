/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:50:00 by jnovais           #+#    #+#             */
/*   Updated: 2026/02/10 11:44:38 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_cd_error(char *msg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putendl_fd(msg, 2);
}

static void	print_cd_errno(char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(strerror(errno), 2);
}

static char	*get_target_path(char **args, char **envp, int *print_path)
{
	char	*path;

	*print_path = 0;
	if (!args[1])
	{
		path = ft_getenv("HOME", envp);
		if (!path)
		{
			print_cd_error("HOME not set");
			return (NULL);
		}
		return (path);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = ft_getenv("OLDPWD", envp);
		if (!path)
		{
			print_cd_error("OLDPWD not set");
			return (NULL);
		}
		*print_path = 1;
		return (path);
	}
	return (ft_strdup(args[1]));
}

int	fd_cd(char **args, char ***envp, int fd_out)
{
	char	*path;
	char	*oldpwd;
	char	*cwd;
	int		print_path;

	if (!args || !envp)
		return (1);
	if (args[1] && args[2])
	{
		print_cd_error("too many arguments");
		return (1);
	}
	path = get_target_path(args, *envp, &print_path);
	if (!path)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		oldpwd = ft_getenv("PWD", *envp);
	if (chdir(path) != 0)
	{
		print_cd_errno(args[1]);
		free(path);
		free(oldpwd);
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (oldpwd)
		env_set(envp, "OLDPWD", oldpwd);
	if (cwd)
		env_set(envp, "PWD", cwd);
	if (print_path && fd_out >= 0)
		ft_putendl_fd(path, fd_out);
	free(oldpwd);
	free(cwd);
	free(path);
	return (0);
}
