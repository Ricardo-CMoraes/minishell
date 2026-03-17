/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 22:39:58 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/17 00:51:07 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_output_file(t_cmd *node, char *filename, t_token_type type)
{
	int	fd;

	if (node && node->invalid)
        return ;
	if (type == RED_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		set_error(filename, node, 1);
		return ;
	}
	if (node->invalid)
	{
		close(fd);
		return ;
	}
	if (node->fd_out != 1)
		close(node->fd_out);
	node->fd_out = fd;
}

void	open_input_file(t_cmd *node, char *path, t_token_type type)
{
	int	fd;
	
	if (node && node->invalid)
        return ;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		set_error(path, node, 1);
	else
	{
		if (node->fd_in > 0)
			close(node->fd_in);
		node->fd_in = fd;
	}
	if (type == HERE_DOC && path)
		unlink(path);
}

void	handle_redirections(t_cmd *node, t_token **tokens)
{
	t_token_type	type;

	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!(*tokens))
	{
		syntax_error_message(NULL, node, 2);
		return ;
	}
	if (((*tokens)->type != WORD))
	{
		syntax_error_message((*tokens)->value, node, 2);
		return ;
	}
	if (type == RED_OUT || type == APPEND)
		open_output_file(node, (*tokens)->value, type);
	else if (type == RED_IN)
		open_input_file(node, (*tokens)->value, RED_IN);
	else if (type == HERE_DOC)
		open_input_file(node, (*tokens)->hdoc_file, HERE_DOC);
	*tokens = (*tokens)->next;
}
