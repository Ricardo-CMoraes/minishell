/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:53:16 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/12 01:25:40 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

static void print_commands(t_cmd *head)
{
    int i;
    int cmd_count = 0;

    while (head)
    {
        printf("\n--- COMANDO %d ---\n", ++cmd_count);
        printf("FD IN: %d\n", head->fd_in);
        printf("FD OUT: %d\n", head->fd_out);
		printf("INVALID: %d\n", head->invalid);
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
	env.envp = copy_environment(envp);
	while (1)
	{
		input = readline("minishell > ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		tokens = lexer(input);
		if (strcmp(input, "exit") == 0)
		{
			free_tokens(tokens);
			free(input);
			break ;
		}
		printf("\n---ANTES DE EXPANDIR---\n");
		temp = tokens;
		while (temp)
		{
			printf("Value: %s\tType: %d\n", temp->value, temp->type);
			temp = temp->next;
		}
		temp = tokens;
		expander(temp, env);
		printf("\n---APÓS EXPANDIR---\n");
		temp = tokens;
		while (temp)
		{
			printf("Value: %s\tType: %d\n", temp->value, temp->type);
			temp = temp->next;
		}
		expander(tokens, env);
		retokenizer(&tokens);
		printf("\n---APÓS RETOKENIZER---\n");
		temp = tokens;
		while (temp)
		{
			printf("Value: %s\tType: %d\n", temp->value, temp->type);
			temp = temp->next;
		}
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

		if (cmds)
			execute_pipeline(cmds, &env.envp);
		//free_commands(cmds);
		free_tokens(tokens);
		free_commands(cmds);
		free(input);
	}
	return (0);
}


// TO DO
// 1. Criar expansão do $?
// 2. Checar leak de FD abertos
// 3. Estudar mais sobre redirections.
// 4. Realizar mais testes individuais sobre redirections
// 5. 
//
//
//
//
//
