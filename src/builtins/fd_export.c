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

static int	env_cmp(const char *a, const char *b)
{
	size_t	la;
	size_t	lb;
	size_t	n;

	la = ft_strlen(a);
	lb = ft_strlen(b);
	if (la > lb)
		n = la;
	else
		n = lb;
	n = n + 1;
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

static void	sort_env(char **sorted, int count)
{
	int		i;
	int		j;
	char	*tmp;

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
}

static void	print_export(char **envp, int fd_out)
{
	int		i;
	int		count;
	char	**sorted;

	count = env_size(envp);
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
	sort_env(sorted, count);
	i = 0;
	while (i < count)
		print_export_entry(sorted[i++], fd_out);
	free(sorted);
}

int	fd_export(char **args, char ***envp, int fd_out)
{
	int	i;
	int	status;

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
		status |= process_export_arg(args[i++], envp);
	return (status);
}
