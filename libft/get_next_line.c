/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdcm <rdcm@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 12:54:48 by rida-cos          #+#    #+#             */
/*   Updated: 2025/10/12 22:21:55 by rdcm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_two(char **buf1, char **buf2)
{
	if (buf1 && *buf1)
	{
		free(*buf1);
		*buf1 = NULL;
	}
	if (buf2 && *buf2)
	{
		free(*buf2);
		*buf2 = NULL;
	}
}

char	*read_file(char *buffer, int fd)
{
	ssize_t	bytes;
	char	*temp_buf;
	char	*new_buf;

	bytes = 1;
	while (!ft_strchr(buffer, '\n') && bytes > 0)
	{
		temp_buf = malloc(BUFFER_SIZE + 1);
		if (!temp_buf)
			return (0);
		bytes = read(fd, temp_buf, BUFFER_SIZE);
		if (bytes < 0)
		{
			free_two(&temp_buf, &buffer);
			return (0);
		}
		temp_buf[bytes] = '\0';
		new_buf = ft_strjoin(buffer, temp_buf);
		free_two(&temp_buf, &buffer);
		buffer = new_buf;
		if (!buffer)
			return (0);
	}
	return (buffer);
}

char	*update_buffer(char *buffer, char **line)
{
	char	*next_line_pos;
	char	*new_buffer;

	if (!buffer || buffer[0] == '\0')
	{
		free(buffer);
		return (NULL);
	}
	next_line_pos = ft_strchr(buffer, '\n');
	if (!next_line_pos)
	{
		*line = ft_strdup(buffer);
		free(buffer);
		return (NULL);
	}
	*line = ft_substr(buffer, 0, next_line_pos - buffer + 1);
	if (ft_strlen(next_line_pos + 1) == 0)
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = ft_strdup(next_line_pos + 1);
	free(buffer);
	return (new_buffer);
}

char	*get_next_line(int fd)
{
	static char	*main_buf;
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!main_buf)
	{
		main_buf = ft_strdup("");
		if (!main_buf)
			return (0);
	}
	main_buf = read_file(main_buf, fd);
	if (!main_buf)
		return (NULL);
	main_buf = update_buffer(main_buf, &line);
	if (!main_buf)
		main_buf = NULL;
	return (line);
}

// #include <fcntl.h>

// int	main(int argc, char *argv[])
// {
// 	char	*linha;
// 	int		fd;

// 	if (argc != 2)
// 	{
// 		printf("Use: %s <arquivo>\n", argv[0]);
// 		return (1);
// 	}

// 	fd = open(argv[1], O_RDONLY);
// 	if (fd < 0)
// 	{
// 		printf("erro ao abrir arquivo");
// 		return (1);
// 	}

// 	int	i = 0;
// 	while (i < 9)
// 	{
// 		linha = get_next_line(fd);
// 		printf("\nmain_line:\t%s", linha);
// 		i++;
// 	}
// 	// linha = get_next_line(fd);
// 	// printf("%s", linha);
// }
