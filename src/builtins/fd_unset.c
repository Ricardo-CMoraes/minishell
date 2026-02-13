/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:50:00 by jnovais           #+#    #+#             */
/*   Updated: 2026/02/04 20:50:00 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_unset_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

int	fd_unset(char **args, char ***envp)
{
	int	i;
	int	status;

	if (!args || !envp || !*envp)
		return (0);
	status = 0;
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '=') || !env_is_valid_name(args[i]))
		{
			print_unset_error(args[i]);
			status = 1;
		}
		else
			env_unset(*envp, args[i]);
		i++;
	}
	return (status);
}
