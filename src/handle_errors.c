/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:29:39 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/13 00:45:27 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error_message(char *token_value)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token_value)
		ft_putstr_fd(token_value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
}

void	set_error(const char *s, t_cmd *node, int status_error)
{
	if (s)
		perror(s);
	if (node)
		node->invalid = 1;
	g_exit_status = status_error;
}
