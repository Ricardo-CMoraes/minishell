/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:00:00 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/04 20:30:00 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	fd_env(char **envp, int fd_out)
{
	int	i;

	if (!envp || fd_out < 0)
		return (0);
	i = 0;
	while (envp[i])
	{
		write(fd_out, envp[i], ft_strlen(envp[i]));
		write(fd_out, "\n", 1);
		i++;
	}
	return (0);
}
