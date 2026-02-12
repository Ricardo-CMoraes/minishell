/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 12:00:00 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/04 20:30:00 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	fd_pwd(int fd_out)
{
	char	*cwd;

	if (fd_out < 0)
		return (1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	write(fd_out, cwd, ft_strlen(cwd));
	write(fd_out, "\n", 1);
	free(cwd);
	return (0);
}
