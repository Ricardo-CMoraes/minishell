/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 21:00:17 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/01 18:41:07 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_syntax(t_token *tmp)
{
	if (!tmp->next || tmp->next->type == PIPE)
	{
		if (tmp->next)
			syntax_error_message(tmp->next->value, NULL, 2);
		else
			syntax_error_message(NULL, NULL, 2);
		return (1);
	}
	return (0);
}

int	redirect_syntax(t_token *tmp)
{
	if (!tmp->next || tmp->next->type != WORD)
	{
		if (tmp->next)
			syntax_error_message(tmp->next->value, NULL, 2);
		else
			syntax_error_message(NULL, NULL, 2);
		return (1);
	}
	return (0);
}

int	check_syntax(t_token	*tokens)
{
	t_token	*tmp;

	if (!tokens)
		return (0);
	if (tokens->type == PIPE)
	{
		syntax_error_message(tokens->value, NULL, 2);
		return (1);
	}
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (pipe_syntax(tmp))
				return (1);
		}
		else if (tmp->type >= RED_OUT && tmp->type <= HERE_DOC)
		{
			if (redirect_syntax(tmp))
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
