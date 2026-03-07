/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_environment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 22:30:49 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/13 00:31:56 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env(char **envp, int limit)
{
	int	i;

	i = 0;
	while (i < limit)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	env_size(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

char	**copy_environment(char **envp)
{
	char	**new_env;
	int		size;
	int		i;

	size = env_size(envp);
	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			free_env(new_env, i);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}
