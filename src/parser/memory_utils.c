/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 09:19:57 by jnovais           #+#    #+#             */
/*   Updated: 2026/03/08 00:27:50 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		free_arr(arr);
	if (input)
		free(input);
}
