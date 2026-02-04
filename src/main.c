/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:53:16 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/01 23:12:20 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void print_commands(t_cmd *head)
{
    int i;
    int cmd_count = 0;

    while (head)
    {
        printf("\n--- COMANDO %d ---\n", ++cmd_count);
        printf("FD IN: %d\n", head->fd_in);
        printf("FD OUT: %d\n", head->fd_out);
        i = 0;
        while (head->args && head->args[i])
        {
            printf("Arg[%d]: %s\n", i, head->args[i]);
            i++;
        }
        head = head->next;
    }
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_token	*tokens;
	t_setup	env;
	t_token	*temp;
	t_cmd	*cmds;

	(void)argc;
	(void)argv;
	env.envp = envp;
	while (1)
	{
		input = readline("minishell > ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		tokens = lexer(input);
		expander(tokens, env);
		retokenizer(&tokens);
		remove_quotes(tokens);
		printf("\n---APÓS REMOVER QUOTES---\n");
		temp = tokens;
		while (temp)
		{
			printf("Value: %s\tType: %d\n", temp->value, temp->type);
			temp = temp->next;
		}
		cmds = build_commands(tokens);
		print_commands(cmds);
		//free_commands(cmds);
		free_tokens(tokens);
		free(input);
	}
	return (0);
}


// TO DO
// 1. implementar handle redirections
// 2. incluir free do cmds
// 3. 
// 4. 
//
//
//
//
//
