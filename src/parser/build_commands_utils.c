/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_commands_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 21:26:29 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/11 01:13:28 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_cmd_node(void)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->args = NULL;
	node->fd_in = 0;
	node->fd_out = 1;
	node->next = NULL;
	node->invalid = 0;
	return (node);
}

void	add_cmd(t_cmd *new_node, t_cmd **head)
{
	t_cmd	*temp;

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

int	count_args(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens && (tokens->type != PIPE))
	{
		if (is_redirect(tokens->type))
		{
			tokens = tokens->next;
			if (tokens)
				tokens = tokens->next;
		}
		else
		{
			i++;
			tokens = tokens->next;
		}
	}
	return (i);
}

int	is_redirect(t_token_type type)
{
	if (type == RED_OUT || type == RED_IN
		|| type == APPEND || type == HERE_DOC)
	{
		return (1);
	}
	return (0);
}

void	free_commands(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
		{
			i = 0;
			while (cmds->args[i])
				free(cmds->args[i++]);
			free(cmds->args);
		}
		if (cmds->fd_in != 0)
			close(cmds->fd_in);
		if (cmds->fd_out != 1)
			close(cmds->fd_out);
		free(cmds);
		cmds = tmp;
	}
}
