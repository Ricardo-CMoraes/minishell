/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:50:00 by jnovais           #+#    #+#             */
/*   Updated: 2026/02/04 20:50:00 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_count(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

static int	env_find(char **envp, const char *key)
{
	int		i;
	size_t	len;

	if (!envp || !key)
		return (-1);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	*build_env_entry(const char *key, const char *value)
{
	char	*tmp;
	char	*entry;

	if (!key)
		return (NULL);
	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	if (!value)
		return (tmp);
	entry = ft_strjoin(tmp, value);
	free(tmp);
	return (entry);
}

int	env_is_valid_name(const char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	**env_dup(char **envp)
{
	int		i;
	int		count;
	char	**copy;

	if (!envp)
		return (NULL);
	count = env_count(envp);
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i > 0)
			{
				free(copy[i - 1]);
				i--;
			}
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

int	env_set(char ***envp, const char *key, const char *value)
{
	int		idx;
	int		count;
	char	*entry;
	char	**new_envp;

	if (!envp || !key)
		return (1);
	if (!*envp)
	{
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
	count = env_count(*envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
	{
		free(entry);
		return (1);
	}
	idx = 0;
	while (idx < count)
	{
		new_envp[idx] = (*envp)[idx];
		idx++;
	}
	new_envp[count] = entry;
	new_envp[count + 1] = NULL;
	free(*envp);
	*envp = new_envp;
	return (0);
}

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
