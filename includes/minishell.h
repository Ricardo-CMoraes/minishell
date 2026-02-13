/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 12:37:09 by rida-cos          #+#    #+#             */
/*   Updated: 2026/02/11 00:55:02 by rida-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

extern int g_exit_status;

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
	t_token_type	type;
	char			*value;
	char			*hdoc_file;
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
	int				invalid;
	struct s_cmd	*next;
}	t_cmd;

//libft/libft.c
size_t		ft_strlen(const char *str);
int	ft_strncmp(const char *s1, const char *s2, size_t n);

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
void	retokenizer(t_token **tokens);
void	split_and_relink(t_token *token);

//remove_quotes.c
char	*remove_quote(char *str);
void	remove_quotes(t_token *tokens);

//build_commands
char	**fill_args(t_token **tokens, t_cmd *new_node);
t_cmd	*build_commands(t_token *tokens);

//build_commands_utils.c
t_cmd	*create_cmd_node();
void	add_cmd(t_cmd *new_node, t_cmd **head);
int		count_args(t_token *tokens);
int		is_redirect(t_token_type type);
void	free_commands(t_cmd *cmds);

// handle_redirections
void	open_output_file(t_cmd *node, char *filename, t_token_type type);
void	open_input_file(t_cmd *node, char *path, t_token_type type);
void	handle_redirections(t_cmd *node, t_token **tokens);

<<<<<<< HEAD

//handle_errors.c
void	syntax_error_message(char *token_value);
void	set_error(const char *s, t_cmd *node, int status_error);

//here_doc.c
char	*generate_tmp_filename(int index);
void	handle_heredoc_creation(t_token *delimiter_token, int index);
void	process_all_heredocs(t_token *tokens);

//copy_environment.c
int		env_size(char **envp);
char	**copy_environment(char **envp);
#endif
=======
//executor.c
int		execute_cmd(t_cmd *cmd, char **envp);

//builtins
int		is_builtin(const char *cmd);
int		execute_builtin(t_cmd *cmd, char ***envp);
int		fd_echo(char **args, int fd_out);
int		fd_env(char **envp, int fd_out);
int		fd_pwd(int fd_out);
int		fd_cd(char **args, char ***envp, int fd_out);
int		fd_export(char **args, char ***envp, int fd_out);
int		fd_unset(char **args, char ***envp);
int		fd_exit(char **args);

//env utils
char	**env_dup(char **envp);
int		env_set(char ***envp, const char *key, const char *value);
int		env_unset(char **envp, const char *key);
int		env_is_valid_name(const char *name);

//path.c
char	*get_dir(char *path, char *cmd);
char	*find_cmd_path(char *cmd, char **envp);

#endif
>>>>>>> feature/exec
