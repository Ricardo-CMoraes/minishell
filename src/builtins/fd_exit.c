/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:50:00 by jnovais           #+#    #+#             */
/*   Updated: 2026/02/04 20:50:00 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	is_numeric(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	print_exit_error(char *msg, char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(msg, 2);
}

static int	is_overflow(char *s)
{
	int		i;
	int		neg;
	long	n;
	long	digit;

	i = 0;
	neg = 0;
	n = 0;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			neg = 1;
		i++;
	}
	while (s[i])
	{
		digit = s[i] - '0';
		if (neg == 0 && n > (9223372036854775807 - digit) / 10)
			return (1);
		if (neg == 1 && n > (long)(9223372036854775808UL) / 10)
			return (1);
		n = n * 10 + digit;
		i++;
	}
	return (0);
}

int	fd_exit(char **args)
{
	long	n;

	write(2, "exit\n", 5);
	if (!args || !args[1])
		exit(0);
	if (!is_numeric(args[1]))
	{
		print_exit_error("numeric argument required", args[1]);
		exit(255);
	}
	if (args[2])
	{
		print_exit_error("too many arguments", NULL);
		return (1);
	}
	if (is_overflow(args[1]))
	{
		print_exit_error("numeric argument required", args[1]);
		exit(255);
	}
	n = ft_atoi(args[1]);
	exit((int)(n & 255));
}
