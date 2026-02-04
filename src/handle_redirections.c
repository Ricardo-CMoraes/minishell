/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirections.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 22:39:58 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/01 22:55:50 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_redirections(t_cmd *node, t_token **tokens)
{
	t_token_type type;

	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!(*tokens))
		return ;
	if (type == RED_OUT)
	{
		if (node->fd_out != 1)
			close(node->fd_out);
		node->fd_out = open((*tokens)->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (type == APPEND)
	{
		if (node->fd_out != 1)
			close(node->fd_out);
		node->fd_out = open((*tokens)->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else if (type == RED_IN)
	{
		if (node->fd_in != 0)
			close(node->fd_in);
		node->fd_in = open((*tokens)->value, O_RDONLY);
		if (node->fd_in == -1)
		{
			perror((*tokens)->value);
		}
	}
	*tokens = (*tokens)->next;
}