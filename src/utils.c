/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 13:06:57 by rida-cos          #+#    #+#             */
/*   Updated: 2026/01/29 23:12:10 by rida-cos         ###   ########.fr       */
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
	return (node);
}

int	is_space(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	free_tokens(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = temp;
	}
}

int	update_state(char c, int state)
{
	if (c == '\'' && state == OUT_QUOTE)
		state = IN_SQUOTE;
	else if (c == '\"' && state == OUT_QUOTE)
		state = IN_DQUOTE;
	else if ((c == '\'' && state == IN_SQUOTE)
		|| (c == '\"' && state == IN_DQUOTE))
		state = OUT_QUOTE;
	return (state);
}

void	free_arr(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while(array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
