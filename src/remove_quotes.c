/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 00:22:14 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/28 21:47:11 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quote(char *str)
{
	char	*new_str;
	int		i;
	int		j;
	t_state	state;
	t_state	next_state;

	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	state = OUT_QUOTE;
	while (str[i])
	{
		next_state = update_state(str[i], state);
		if (state != next_state)
			state = next_state;
		else
			new_str[j++] = str[i];
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	remove_quotes(t_token *tokens)
{
	t_token	*curr;
	char	*new_value;

	curr = tokens;
	while (curr)
	{
		if (curr->type == WORD)
		{
			new_value = remove_quote(curr->value);
			free(curr->value);
			curr->value = new_value;
		}
		curr = curr->next;
	}
}
