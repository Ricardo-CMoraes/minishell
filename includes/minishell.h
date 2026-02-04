/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 12:37:09 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/03 20:43:58 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
 #include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	RED_OUT,
	RED_IN,
	APPEND,
	HERE_DOC,
	ENV
}	t_token_type;

typedef enum e_state
{
	OUT_QUOTE,
	IN_SQUOTE,
	IN_DQUOTE
}	t_state;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_setup
{
	char	**envp;
	t_state	state;
}	t_setup;

typedef struct s_cmd
{
	char			**args;
	int				fd_in;
	int				fd_out;
	struct s_cmd	*next;
	
}	t_cmd;

//libft/libft.c
size_t		ft_strlen(const char *str);

//utils.c
t_token		*create_token(char *value, t_token_type type);
int			is_space(char c);
int			is_operator(char c);
void		free_tokens(t_token *head);
int			update_state(char c, int state);
char		*clean_quotes(char *str);
void		free_arr(char **array);

//lexer.c
void		add_token(t_token *new_token, t_token **head, int *i);
void		handler_redirection(char *input, t_token **head, int *i);
void		handle_word(char *input, t_token **head, int *i);
t_token		*lexer(char *input);

//expander.c
char		*append_char(char *str, char c);
char		*expand_and_join(char *new_str, char *str, int *i, t_setup env);
char		*handler_expansion(char *str, t_setup env);
void		expander(t_token *tokens, t_setup env);

//expander_utils.c
char		*extract_var_name(char *str);
void		prepare_to_split(char *var_value, int state);
char		*ft_getenv(char *name, char **env);

// retokenizer.c
void retokenizer(t_token **tokens);
void    split_and_relink(t_token *token);

//remove_quotes.c
char *remove_quote(char *str);
void	remove_quotes(t_token *tokens);

//build_commands
t_cmd	*create_cmd_node();
int		count_args(t_token *tokens);
char **fill_args(t_token **tokens, t_cmd *new_node);
void	add_cmd(t_cmd *new_node, t_cmd **head);
t_cmd	*build_commands(t_token *tokens);

//build_commands_utils.c
int is_redirect(t_token_type type);

// handle_redirections
void handle_redirections(t_cmd *node, t_token **tokens);

#endif