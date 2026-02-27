/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 21:26:36 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/13 00:48:48 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**fill_args(t_token **tokens, t_cmd *new_node)
{
	int		n_args;
	char	**args;
	int		i;

	n_args = count_args(*tokens);
	args = malloc(sizeof(char *) * (n_args + 1));
	i = 0;
	while (*tokens && (*tokens)->type != PIPE)
	{
		if (is_redirect((*tokens)->type))
		{
			handle_redirections(new_node, tokens);
		}
		else
		{
			args[i] = ft_strdup((*tokens)->value);
			i++;
			*tokens = (*tokens)->next;
		}
	}
	args[i] = NULL;
	return (args);
}

t_cmd	*build_commands(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*new_node;
	t_token	*curr;
	int		pipe_rd;
	int		fd_pipe[2];

	if (g_exit_status == 130)
		g_exit_status = 0;
	process_all_heredocs(tokens);
	if (g_exit_status == 130)
		return (NULL);
	head = NULL;
	curr = tokens;
	pipe_rd = 0;
	while (curr)
	{
		new_node = create_cmd_node();
		new_node->fd_in = pipe_rd;
		new_node->args = fill_args(&curr, new_node);
		add_cmd(new_node, &head);
		if (curr && curr->type == PIPE)
		{
			pipe(fd_pipe);
			if (new_node->fd_out == 1)
				new_node->fd_out = fd_pipe[1];
			else
				close(fd_pipe[1]);
			pipe_rd = fd_pipe[0];
			curr = curr->next;
		}
		else
			pipe_rd = 0;
	}
	return (head);
}
