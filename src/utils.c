/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 13:06:57 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/04 21:26:51 by rida-cos         ###   ########.fr       */
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

void	free_arr(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_all(char *input, t_token *tokens, t_cmd *cmds, char **arr)
{
	if (tokens)
		free_tokens(tokens);
	if (cmds)
		free_commands(cmds);
	if (arr)
		free_arr(arr); // Sua função que limpa char ** (env ou similares)
	if (input)
		free(input);
}