/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:53:16 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/01 18:37:47 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

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


int	process_input(t_token **tokens, t_cmd **cmds,t_setup *envp)
{
	char *input;

	setup_signals();
	input = readline("minishell > ");
	if (!input)
		return (-1);
	if (!*input)
	{
		free(input);
		return (0);
	}
	add_history(input);
	*tokens = lexer(input);
	free(input);
	if (check_syntax(*tokens) || process_all_heredocs(*tokens))
		return (0);
	expander(*tokens, *envp);
	retokenizer(tokens, NULL);
	remove_quotes(*tokens);
	*cmds = build_commands(*tokens, NULL);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	t_setup	env;
	//t_token	*temp;
	t_cmd	*cmds;
	int process_status;

	(void)argc;
	(void)argv;
	env.envp = copy_environment(envp);
	while (1)
	{
		tokens = NULL;
		cmds = NULL;
		process_status = process_input(&tokens, &cmds, &env);
		if (process_status == -1)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (process_status == 1 && cmds)
			execute_pipeline(cmds, &env.envp);
		unlink_heredocs(tokens);
		free_all(NULL, tokens, cmds, NULL);
	}
	free_all(NULL, NULL, NULL, env.envp);
	return (0);
}


// TO DO
// 1. Reduzir o tamanho da main
// 2. testar a regua
// 3. 
// 4.
//
//
