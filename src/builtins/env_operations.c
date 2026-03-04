/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 09:30:50 by jnovais           #+#    #+#             */
/*   Updated: 2026/03/03 09:30:50 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_create_first(char ***envp,
		const char *key, const char *value)
{
	char	**new_envp;
	char	*entry;

	new_envp = malloc(sizeof(char *) * 2);
	if (!new_envp)
		return (1);
	entry = build_env_entry(key, value);
	if (!entry)
	{
		free(new_envp);
		return (1);
	}
	new_envp[0] = entry;
	new_envp[1] = NULL;
	*envp = new_envp;
	return (0);
}

static int	env_add_new(char ***envp, char *entry)
{
	char	**new_envp;
	int		count;
	int		i;

	count = env_size(*envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
	{
		free(entry);
		return (1);
	}
	i = 0;
	while (i < count)
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	new_envp[count] = entry;
	new_envp[count + 1] = NULL;
	free(*envp);
	*envp = new_envp;
	return (0);
}

int	env_set(char ***envp, const char *key, const char *value)
{
	int		idx;
	char	*entry;

	if (!envp || !key)
		return (1);
	if (!*envp)
		return (env_create_first(envp, key, value));
	idx = env_find(*envp, key);
	if (idx >= 0 && !value)
		return (0);
	entry = build_env_entry(key, value);
	if (!entry)
		return (1);
	if (idx >= 0)
	{
		free((*envp)[idx]);
		(*envp)[idx] = entry;
		return (0);
	}
	return (env_add_new(envp, entry));
}

// int	env_set(char ***envp, const char *key, const char *value)
// {
// 	int		idx;
// 	int		count;
// 	char	*entry;
// 	char	**new_envp;

// 	if (!envp || !key)
// 		return (1);
// 	if (!*envp)
// 	{
// 		new_envp = malloc(sizeof(char *) * 2);
// 		if (!new_envp)
// 			return (1);
// 		entry = build_env_entry(key, value);
// 		if (!entry)
// 		{
// 			free(new_envp);
// 			return (1);
// 		}
// 		new_envp[0] = entry;
// 		new_envp[1] = NULL;
// 		*envp = new_envp;
// 		return (0);
// 	}
// 	idx = env_find(*envp, key);
// 	if (idx >= 0 && !value)
// 		return (0);
// 	entry = build_env_entry(key, value);
// 	if (!entry)
// 		return (1);
// 	if (idx >= 0)
// 	{
// 		free((*envp)[idx]);
// 		(*envp)[idx] = entry;
// 		return (0);
// 	}
// 	count = env_size(*envp);
// 	new_envp = malloc(sizeof(char *) * (count + 2));
// 	if (!new_envp)
// 	{
// 		free(entry);
// 		return (1);
// 	}
// 	idx = 0;
// 	while (idx < count)
// 	{
// 		new_envp[idx] = (*envp)[idx];
// 		idx++;
// 	}
// 	new_envp[count] = entry;
// 	new_envp[count + 1] = NULL;
// 	free(*envp);
// 	*envp = new_envp;
// 	return (0);
// }

int	env_unset(char **envp, const char *key)
{
	int	idx;

	if (!envp || !key)
		return (0);
	idx = env_find(envp, key);
	if (idx < 0)
		return (0);
	free(envp[idx]);
	while (envp[idx + 1])
	{
		envp[idx] = envp[idx + 1];
		idx++;
	}
	envp[idx] = NULL;
	return (0);
}
