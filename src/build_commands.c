/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 21:26:36 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/28 19:53:12 by rida-cos         ###   ########.fr       */
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

t_cmd	*build_commands(t_token *tokens, t_cmd *head)
{
	t_cmd	*new_node;
	int		pipe_rd;
	int		fd_pipe[2];

	pipe_rd = 0;
	while (tokens)
	{
		new_node = create_cmd_node();
		new_node->fd_in = pipe_rd;
		new_node->args = fill_args(&tokens, new_node);
		add_cmd(new_node, &head);
		if (tokens && tokens->type == PIPE)
		{
			pipe(fd_pipe);
			if (new_node->fd_out == 1)
				new_node->fd_out = fd_pipe[1];
			else
				close(fd_pipe[1]);
			pipe_rd = fd_pipe[0];
			tokens = tokens->next;
		}
		else
			pipe_rd = 0;
	}
	return (head);
}
