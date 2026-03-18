/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 13:06:57 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/08 01:51:55 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = value;
	node->type = type;
	node->next = NULL;
	node->hdoc_file = NULL;
	return (node);
}

void	free_tokens(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head->next;
		if (head->value)
			free(head->value);
		if (head->hdoc_file)
			free(head->hdoc_file);
		free(head);
		head = temp;
	}
}

void	set_null(t_token **tokens, t_cmd **cmds)
{
	*tokens = NULL;
	*cmds = NULL;
}

void	set_env_struct(t_setup *env, char **envp, char *shell_name)
{
	env->shell_name = ft_strdup(shell_name);
	env->envp = copy_environment(envp);
	g_exit_status = 0;
}
