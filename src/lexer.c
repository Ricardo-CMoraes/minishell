/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 12:45:52 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/01 20:38:39 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_token *new_token, t_token **head, int *i)
{
	t_token	*temp;

	if (!new_token || !head)
		return ;
	if (*head == NULL)
		*head = new_token;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
	}
	if (i != NULL)
		*i += ft_strlen(new_token->value);
}

void	handler_redirection(char *input, t_token **head, int *i)
{
	if (input[*i] == '>')
	{
		if (input[*i + 1] == '>') // is a APPEND
			add_token(create_token(ft_strdup(">>"), APPEND), head, i);
		else //RED_OUT
			add_token(create_token(ft_strdup(">"), RED_OUT), head, i);
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<') // is a HRE_DOC
			add_token(create_token(ft_strdup(">>"), HERE_DOC), head, i);
		else //RED_IN
			add_token(create_token(ft_strdup("<"), RED_IN), head, i);
	}
}

static int	get_word_end(char *input, int i, t_state *state)
{
	while (input[i])
	{
		if (*state == OUT_QUOTE)
		{
			if (input[i] == '\'')
				*state = IN_SQUOTE;
			else if (input[i] == '\"')
				*state = IN_DQUOTE;
			else if (is_space(input[i]) || is_operator(input[i]))
				break ;
		}
		else if (*state == IN_SQUOTE)
		{
			if (input[i] == '\'')
				*state = OUT_QUOTE;
		}
		else if (*state == IN_DQUOTE)
		{
			if (input[i] == '\"')
				*state = OUT_QUOTE;
		}
		i++;
	}
	return (i);
}

void	handle_word(char *input, t_token **head, int *i)
{
	int		start;
	int		end;
	t_state	state;

	state = OUT_QUOTE;
	start = *i;
	end = get_word_end(input, *i, &state);
	if (state != OUT_QUOTE)
	{
		printf("Error: open quotes\n");
		free_tokens(*head);
		*head = NULL;
		return ;
	}
	add_token(create_token(
			ft_substr(input, start, end - start), WORD), head, NULL);
	*i = end;
}

t_token	*lexer(char *input)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && is_space(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '|')
			add_token(create_token(ft_strdup("|"), PIPE), &head, &i);
		else if (input[i] == '<' || input[i] == '>')
			handler_redirection(input, &head, &i);
		else
			handle_word(input, &head, &i);
	}
	return (head);
}
