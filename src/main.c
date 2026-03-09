/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 23:53:16 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/08 01:57:58 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

int	process_input(t_token **tokens, t_cmd **cmds, t_setup *env)
{
	char	*input;

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
	if ((*tokens == NULL) || check_syntax(*tokens)
		|| process_all_heredocs(*tokens))
		return (0);
	expander(*tokens, *env);
	retokenizer(tokens, NULL);
	remove_quotes(*tokens);
	*cmds = build_commands(*tokens, NULL);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_token	*tokens;
	t_setup	env;
	t_cmd	*cmds;

	if (argc > 1)
		return (write(2, "Error: Minishell does not accept arguments\n", 44));
	set_env_struct(&env, envp, argv[0]);
	while (1)
	{
		set_null(&tokens, &cmds);
		env.input_status = process_input(&tokens, &cmds, &env);
		if (env.input_status == -1)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (env.input_status == 1 && cmds)
			execute_pipeline(cmds, &env.envp);
		unlink_heredocs(tokens);
		free_all(NULL, tokens, cmds, NULL);
	}
	free_all(env.shell_name, NULL, NULL, env.envp);
	return (0);
}

// TO DO
// 1. Testar regua da 42
// 2. Escrever rascunho do README.md