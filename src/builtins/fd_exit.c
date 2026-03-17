/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:50:00 by jnovais           #+#    #+#             */
/*   Updated: 2026/03/17 02:29:00 by rida-cos         ###   ########.fr       */
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
	int		len;
	int		neg;
	char	*max;

	neg = 0;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			neg = 1;
		s++;
	}
	while (*s == '0' && *(s + 1))
		s++;
	len = ft_strlen(s);
	if (len > 19)
		return (1);
	if (len < 19)
		return (0);
	if (neg)
		max = "9223372036854775808";
	else
		max = "9223372036854775807";
	if (ft_strncmp(s, max, 19) > 0)
		return (1);
	return (0);
}

int	fd_exit(char **args)
{
	long	n;

	ft_putstr_fd("exit\n", 1);
	if (!args || !args[1])
		exit(0);
	if (!is_numeric(args[1]) || is_overflow(args[1]))
	{
		print_exit_error("numeric argument required", args[1]);
		exit(2);
	}
	if (args[2])
	{
		print_exit_error("too many arguments", NULL);
		return (1);
	}
	n = ft_atoi(args[1]);
	exit((int)(n & 255));
}
