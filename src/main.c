/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:53:16 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/28 22:45:53 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

// static void print_commands(t_cmd *head)
// {
// 	int i;
// 	int cmd_count = 0;

// 	while (head)
// 	{
// 		printf("\n--- COMANDO %d ---\n", ++cmd_count);
// 		printf("FD IN: %d\n", head->fd_in);
// 		printf("FD OUT: %d\n", head->fd_out);
// 		printf("INVALID: %d\n", head->invalid);
// 		i = 0;
// 		while (head->args && head->args[i])
// 		{
// 			printf("Arg[%d]: %s\n", i, head->args[i]);
// 			i++;
// 		}
// 		head = head->next;
// 	}
// }

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
		add_history(input);
		tokens = lexer(input);
		if (check_syntax(tokens))
		{
			free_all(input, tokens, NULL, NULL);
			continue;
		}
		if (process_all_heredocs(tokens))
		{
			unlink_heredocs(tokens);
			free_all(input, tokens, NULL, NULL);
			continue;
		}

		expander(tokens, env);
		retokenizer(&tokens, NULL);
		remove_quotes(tokens);
		cmds = build_commands(tokens, NULL);
		if (cmds)
		{
			execute_pipeline(cmds, &env.envp);
		}
		unlink_heredocs(tokens);
		free_all(input, tokens, cmds, NULL);
	}
	free_all(NULL, NULL, NULL, env.envp);
	return (0);
}


// TO DO
// 1. Finalizar Ctrl+C
// 2. Finalizar Ctrl+D
// 3. Implementar ;
// 3. Incluir o add_history()ç
//
//
