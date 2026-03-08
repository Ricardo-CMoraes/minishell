/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rida-cos <ric.costamoraes@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 12:37:09 by rida-cos          #+#    #+#             */
/*   Updated: 2026/03/08 02:41:05 by rida-cos         ###   ########.fr       */
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
# include <signal.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>

extern int	g_exit_status;

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
	int		input_status;
	char	*shell_name;
}	t_setup;

typedef struct s_cmd
{
	char			**args;
	int				fd_in;
	int				fd_out;
	int				invalid;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_exec_ctx
{
	t_cmd	*cmds;
	t_cmd	*current;
	pid_t	last_pid;
	int		executed_any;
	int		had_invalid;
	char	***envp;
}	t_exec_ctx;

typedef struct s_cd_ctx
{
	char	***envp;
	char	*path;
	int		print_path;
	int		fd_out;
}	t_cd_ctx;

//libft/libft.c
size_t		ft_strlen(const char *str);
int			ft_strncmp(const char *s1, const char *s2, size_t n);

//src/builtins/builtins.c
int			is_builtin(const char *cmd);
int			execute_builtin(t_cmd *cmd, char ***envp);

//src/builtins/env_operations.c
int			env_set(char ***envp, const char *key, const char *value);
int			env_unset(char **envp, const char *key);

//src/builtins/env_utils.c
int			env_find(char **envp, const char *key);
char		*build_env_entry(const char *key, const char *value);
int			env_is_valid_name(const char *name);
char		**env_dup(char **envp);

//src/builtins/fd_cd.c
int			fd_cd(char **args, char ***envp, int fd_out);

//src/builtins/fd_echo.c
int			fd_echo(char **args, int fd_out);

//src/builtins/fd_env.c
int			fd_env(char **envp, int fd_out);

//src/builtins/fd_exit.c
int			fd_exit(char **args);

//src/builtins/fd_export_helper.c
int			process_export_arg(char *arg, char ***envp);

//src/builtins/fd_export.c
int			fd_export(char **args, char ***envp, int fd_out);

//src/builtins/fd_pwd.c
int			fd_pwd(int fd_out);

//src/builtins/fd_unset.c
int			fd_unset(char **args, char ***envp);

//src/parser/build_commands_utils.c
t_cmd		*create_cmd_node(void);
void		add_cmd(t_cmd *new_node, t_cmd **head);
int			count_args(t_token *tokens);
int			is_redirect(t_token_type type);
void		free_commands(t_cmd *cmds);

//src/parser/build_commands.c
char		**fill_args(t_token **tokens, t_cmd *new_node);
t_cmd		*build_commands(t_token *tokens, t_cmd *head);

//src/parser/check_syntax.c
int			pipe_syntax(t_token *tmp);
int			redirect_syntax(t_token *tmp);
int			check_syntax(t_token	*tokens);

//src/parser/copy_environment.c
int			env_size(char **envp);
char		**copy_environment(char **envp);

//src/parser/expander_utils.c
int			update_state(char c, int state);
char		*extract_var_name(char *str);
void		prepare_to_split(char *var_value, int state);
char		*ft_getenv(char *name, char **env);

//src/parser/expander.c
char		*append_char(char *str, char c);
char		*expand_and_join(char *new_str, char *str, int *i, t_setup env);
char		*handler_expansion(char *str, t_setup env);
void		expander(t_token *tokens, t_setup env);

//src/parser/handle_errors.c
void		syntax_error_message(char *token_value, t_cmd *node,
				int status_error);
void		set_error(const char *s, t_cmd *node, int status_error);
void		unlink_heredocs(t_token *tokens);

//src/parser/handle_redirections
void		open_output_file(t_cmd *node, char *filename, t_token_type type);
void		open_input_file(t_cmd *node, char *path, t_token_type type);
void		handle_redirections(t_cmd *node, t_token **tokens);

//src/parser/handle_signal.c
void		handle_sigint(int sig);
void		setup_signals(void);
void		handle_sigint_heredoc(int sig);

//src/parser/here_doc.c
char		*generate_tmp_filename(int index);
void		handle_heredoc_creation(t_token *dlmt_token, int index);
int			process_all_heredocs(t_token *tokens);

//src/parser/lexer_utils.c
int			is_space(char c);
int			is_operator(char c);

//src/parser/lexer.c
void		add_token(t_token *new_token, t_token **head, int *i);
void		handler_redirection(char *input, t_token **head, int *i);
int			handle_word(char *input, t_token **head, int *i);
t_token		*lexer(char *input);

//src/parser/memory_utils.c
void		free_arr(char **array);
void		free_all(char *input, t_token *tokens, t_cmd *cmds, char **arr);

//src/parser/remove_quotes.c
char		*remove_quote(char *str);
void		remove_quotes(t_token *tokens);

//src/parser/retokenizer.c
void		retokenizer(t_token **tokens, t_token *prev);
void		split_and_relink(t_token *token);

//executor_helpers.c
int			handle_cmd_not_found(t_cmd *cmd);
void		setup_pipeline_signals(void);
void		restore_signals_and_wait(void);
int			process_single_builtin(t_cmd *cmd, t_cmd *cmds, char ***envp);
int			process_cmd_loop(t_exec_ctx *ctx);

//executor_pipeline.c
void		handle_pipeline_status(int status, int executed_any,
				int had_invalid);
void		close_and_reset_fds(t_cmd *cmd);
void		close_other_fds(t_cmd *cmds, t_cmd *current);

//executor_utils.c
void		apply_redirections(t_cmd *cmd);
void		child_process(t_cmd *cmd, t_cmd *cmds, char ***envp);
void		handle_execve_error(char *cmd_name, char *path, int status);
pid_t		create_child_process(t_cmd *cmd, t_cmd *cmds, char ***envp);

//executor.c
int			execute_cmd(t_cmd *cmd, char **envp);
void		execute_pipeline(t_cmd *cmds, char ***envp);

//src/main.c
int			process_input(t_token **tokens, t_cmd **cmds, t_setup *envp);

//src/path.c
char		*get_dir(char *path, char *cmd);
char		*find_cmd_path(char *cmd, char **envp);

//src/utils.c
t_token		*create_token(char *value, t_token_type type);
void		free_tokens(t_token *head);
void		set_null(t_token **tokens, t_cmd **cmds);
void		set_env_struct(t_setup *env, char **envp, char *shell_name);

//char		*clean_quotes(char *str);
//void		apply_terminal_settings(void);

#endif
