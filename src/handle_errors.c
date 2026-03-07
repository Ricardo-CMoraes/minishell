/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:29:39 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/28 21:56:39 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error_message(char *token_value, t_cmd *node, int status_error)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token_value)
		ft_putstr_fd(token_value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	if (node)
		node->invalid = 1;
	g_exit_status = status_error;
}

void	set_error(const char *s, t_cmd *node, int status_error)
{
	if (s)
	{
		if (status_error == 2)
			ft_putendl_fd((char *)s, 2);
		else
			perror(s);
	}
	if (node)
		node->invalid = 1;
	g_exit_status = status_error;
}

void	unlink_heredocs(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == HERE_DOC && tokens->next && tokens->next->hdoc_file)
		{
			unlink(tokens->next->hdoc_file);
			free(tokens->next->hdoc_file);
			tokens->next->hdoc_file = NULL;
		}
		tokens = tokens->next;
	}
}
