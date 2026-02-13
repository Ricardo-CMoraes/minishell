/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:50:00 by jnovais           #+#    #+#             */
/*   Updated: 2026/02/04 20:50:00 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	env_count(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

static int	env_cmp(const char *a, const char *b)
{
	size_t	la;
	size_t	lb;
	size_t	n;

	la = ft_strlen(a);
	lb = ft_strlen(b);
	n = (la > lb ? la : lb) + 1;
	return (ft_strncmp(a, b, n));
}

static void	print_export_entry(char *entry, int fd_out)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	ft_putstr_fd("declare -x ", fd_out);
	if (!eq)
	{
		ft_putendl_fd(entry, fd_out);
		return ;
	}
	write(fd_out, entry, eq - entry);
	ft_putstr_fd("=\"", fd_out);
	ft_putstr_fd(eq + 1, fd_out);
	ft_putendl_fd("\"", fd_out);
}

static void	print_export(char **envp, int fd_out)
{
	int		i;
	int		j;
	int		count;
	char	**sorted;
	char	*tmp;

	count = env_count(envp);
	if (count == 0)
		return ;
	sorted = malloc(sizeof(char *) * count);
	if (!sorted)
		return ;
	i = 0;
	while (i < count)
	{
		sorted[i] = envp[i];
		i++;
	}
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (env_cmp(sorted[j], sorted[j + 1]) > 0)
			{
				tmp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < count)
	{
		print_export_entry(sorted[i], fd_out);
		i++;
	}
	free(sorted);
}

static void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

int	fd_export(char **args, char ***envp, int fd_out)
{
	int		i;
	int		status;
	char	*eq;
	char	*key;

	if (!args || !envp)
		return (1);
	if (!args[1])
	{
		print_export(*envp, fd_out);
		return (0);
	}
	status = 0;
	i = 1;
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (eq)
			key = ft_substr(args[i], 0, eq - args[i]);
		else
			key = ft_strdup(args[i]);
		if (!key || !env_is_valid_name(key))
		{
			print_export_error(args[i]);
			status = 1;
		}
		else if (eq)
			env_set(envp, key, eq + 1);
		else
			env_set(envp, key, NULL);
		free(key);
		i++;
	}
	return (status);
}
