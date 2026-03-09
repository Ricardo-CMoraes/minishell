/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 00:50:47 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/01 18:40:09 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*generate_tmp_filename(int index)
{
	char	*num;
	char	*filename;

	num = ft_itoa(index);
	filename = ft_strjoin(".heredoc_tmp_", num);
	free(num);
	return (filename);
}

static void	warning_message(char *delimiter)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
}

static void	read_heredoc_lines(int fd, char *delimiter)
{
	char	*line;
	size_t	dlmt_len;

	dlmt_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (g_exit_status == 130 || !line
			|| (ft_strlen(line) == dlmt_len
				&& ft_strncmp(line, delimiter, dlmt_len) == 0))
		{
			if (g_exit_status != 130 && !line)
				warning_message(delimiter);
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

void	handle_heredoc_creation(t_token *dlmt_token, int index)
{
	char	*temp_name;
	int		temp_fd;
	int		stdin_backup;

	g_exit_status = 0;
	temp_name = generate_tmp_filename(index);
	temp_fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (temp_fd == -1)
	{
		free(temp_name);
		return ;
	}
	signal(SIGINT, handle_sigint_heredoc);
	stdin_backup = dup(0);
	read_heredoc_lines(temp_fd, dlmt_token->value);
	close(temp_fd);
	dup2(stdin_backup, 0);
	close(stdin_backup);
	setup_signals();
	dlmt_token->hdoc_file = temp_name;
}

int	process_all_heredocs(t_token *tokens)
{
	t_token	*curr;
	int		heredoc_count;

	curr = tokens;
	heredoc_count = 0;
	while (curr)
	{
		if (curr->type == HERE_DOC)
		{
			if (curr->next && curr->next->type == WORD)
			{
				handle_heredoc_creation(curr->next, heredoc_count);
				heredoc_count++;
			}
			if (g_exit_status == 130)
				return (1);
		}
		curr = curr->next;
	}
	return (0);
}
