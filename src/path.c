/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnovais <jnovais@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 20:57:22 by jnovais           #+#    #+#             */
/*   Updated: 2026/02/11 09:42:49 by jnovais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_split(char **parts)
{
	size_t	i;

	if (!parts)
		return ;
	i = 0;
	while (parts[i])
		free(parts[i++]);
	free(parts);
}

static char	*build_cmd_path(const char *dir, const char *cmd)
{
	char	*full_path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_path = malloc(len);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, len);
	ft_strlcat(full_path, "/", len);
	ft_strlcat(full_path, cmd, len);
	return (full_path);
}

static char	*return_with_free(char **dirs, char *path)
{
	free_split(dirs);
	return (path);
}

char	*get_dir(char *path, char *cmd)
{
	char	**dirs;
	char	*full_path;
	size_t	i;

	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full_path = build_cmd_path(dirs[i], cmd);
		if (!full_path)
			return (return_with_free(dirs, NULL));
		if (access(full_path, X_OK) == 0)
			return (return_with_free(dirs, full_path));
		free(full_path);
		i++;
	}
	return (return_with_free(dirs, NULL));
}

char	*find_cmd_path(char *cmd, char **envp)
{
	char	*path;
	char	*result;
	int		i;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break;
		}
		i++;
	}
	if (!path)
	{
		return (NULL);
	}
	result = get_dir(path, cmd);
	return (result);
}
