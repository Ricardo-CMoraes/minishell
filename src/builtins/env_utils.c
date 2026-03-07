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

int	env_find(char **envp, const char *key)
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

char	*build_env_entry(const char *key, const char *value)
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

static void	env_dup_cleanup(char **copy, int i)
{
	while (i > 0)
	{
		free(copy[i - 1]);
		i--;
	}
	free(copy);
}

char	**env_dup(char **envp)
{
	int		i;
	int		count;
	char	**copy;

	if (!envp)
		return (NULL);
	count = env_size(envp);
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			env_dup_cleanup(copy, i);
			return (NULL);
		}
		i++;
	}
	copy[count] = NULL;
	return (copy);
}
