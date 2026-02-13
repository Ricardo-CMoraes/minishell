/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:00:00 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/04 20:30:00 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	is_n_option(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] == '\0')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	fd_echo(char **args, int fd_out)
{
	int	i;
	int	print_newline;
	int	first;

	if (!args || fd_out < 0)
		return (1);
	print_newline = 1;
	i = 1;
	while (args[i] && is_n_option(args[i]))
	{
		print_newline = 0;
		i++;
	}
	first = 1;
	while (args[i])
	{
		if (!first)
			write(fd_out, " ", 1);
		write(fd_out, args[i], ft_strlen(args[i]));
		first = 0;
		i++;
	}
	if (print_newline)
		write(fd_out, "\n", 1);
	return (0);
}
