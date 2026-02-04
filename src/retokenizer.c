/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retokenizer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 23:49:41 by rida-cos          #+#    #+#             */
/*   Updated: 2026/01/29 23:14:30 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    split_and_relink(t_token *token)
{
    char **splited;
    int i;
    t_token *next_node;
    t_token *new_node;
    
    splited = ft_split(token->value, 1);
    next_node = token->next;
    free(token->value);
    token->value = ft_strdup(splited[0]);
    i = 1;
    while(splited[i])
    {
        new_node = create_token(ft_strdup(splited[i]), WORD);
        token->next = new_node;
        token = new_node;
        i++;
    }
    token->next = next_node;
    free_arr(splited);
}

void retokenizer(t_token **tokens)
{
    t_token *curr;

    if (!tokens || !*tokens)
        return ;
    curr = *tokens;
    while (curr)
    {
        if (curr->type == WORD && strchr(curr->value, 1))
            split_and_relink(curr);
        curr = curr->next;
    }
}