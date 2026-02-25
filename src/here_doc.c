/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 00:50:47 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/13 00:52:36 by rida-cos         ###   ########.fr       */
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

void	handle_heredoc_creation(t_token *delimiter_token, int index)
{
	char	*temp_name;
	char	*line;
	int		temp_fd;
	char	*delimiter;
	int		stdin_backup;

	g_exit_status = 0;
	stdin_backup = dup(0);
	signal(SIGINT, handle_sigint_heredoc);
	delimiter = delimiter_token->value;
	temp_name = generate_tmp_filename(index);
	temp_fd = open(temp_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (temp_fd == -1)
	{
		close(stdin_backup);		
		return ;
	}
	while (1)
	{
		line = readline("> ");
		if (g_exit_status == 130 || !line || ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			if (g_exit_status != 130 && !line)
				ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", 2);
			free(line);
			break ;
		}
		ft_putendl_fd(line, temp_fd);
		free(line);
	}
	close(temp_fd);
	dup2(stdin_backup, 0);
	close(stdin_backup);
	setup_signals();
	delimiter_token->hdoc_file = temp_name;
}

void	process_all_heredocs(t_token *tokens)
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
				break ;
		}
		curr = curr->next;
	}
}
