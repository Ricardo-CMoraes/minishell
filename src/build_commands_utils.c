/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 21:26:29 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/01 21:46:16 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd *create_cmd_node()
{
	t_cmd *node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->args = NULL;
	node->fd_in = 0;
	node->fd_out = 1;
	node->next = NULL;
	return (node);
}

void add_cmd(t_cmd *new_node, t_cmd **head)
{
	t_cmd *temp;

	if (!new_node | !head)
		return ;
	if (*head == NULL)
		*head = new_node;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

int count_args(t_token *tokens)
{
	int	i;
	
	i = 0;
	while (tokens && (tokens->type != PIPE))
	{
		if (is_redirect(tokens->type))
		{
			tokens = tokens->next; // Pula operador
			if (tokens)
				tokens = tokens->next; // Pula o nome do arquivo
		}
		else
		{
			i++;
			tokens = tokens->next;
		}
	}
	return (i);
}

int is_redirect(t_token_type type)
{
	if (type == RED_OUT || type == RED_IN 
		|| type == APPEND || type == HERE_DOC)
	{
		return (1);
	}
	return (0);
}