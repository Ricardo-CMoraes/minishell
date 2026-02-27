/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:53:16 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/26 21:22:03 by rida-cos         ###   ########.fr       */
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
	//t_token	*temp;
	t_cmd	*cmds;

	(void)argc;
	(void)argv;
	env.envp = copy_environment(envp);
	while (1)
	{
		setup_signals();
		input = readline("minishell > ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		//add_history(input);
		tokens = lexer(input);
		if (check_syntax(tokens))
        {
            free_tokens(tokens);
            free(input);
            continue; // Já deu o erro, volta para o próximo prompt
        }
		if (strcmp(input, "exit") == 0)
		{
			free_tokens(tokens);
			free(input);
			break ;
		}
		expander(tokens, env);
		retokenizer(&tokens);
		remove_quotes(tokens);
		cmds = build_commands(tokens);
		if (cmds == NULL && g_exit_status == 130)
		{
			unlink_heredocs(tokens);
			free_tokens(tokens);
			free(input);
			continue ; 
		}
		if (cmds)
		{
			execute_pipeline(cmds, &env.envp);
		}
		unlink_heredocs(tokens);
		free_tokens(tokens);
		free_commands(cmds);
		free(input);
	}
	free_arr(env.envp);
	return (0);
}


// TO DO
// 2. Checar leak de FD abertos
// 3. Estudar mais sobre redirections.
// 4. Realizar mais testes individuais sobre redirections
// 5. 
//
//
//
//
//
