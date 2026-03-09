NAME = minishell

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -Ilibft -Iincludes

SRC = src/builtins/builtins.c \
		src/builtins/env_operations.c \
		src/builtins/env_utils.c \
		src/builtins/fd_cd.c \
		src/builtins/fd_echo.c \
		src/builtins/fd_env.c \
		src/builtins/fd_exit.c \
		src/builtins/fd_export_helper.c \
		src/builtins/fd_export.c \
		src/builtins/fd_pwd.c \
		src/builtins/fd_unset.c \
		src/parser/build_commands_utils.c \
		src/parser/build_commands.c \
		src/parser/check_syntax.c \
		src/parser/copy_environment.c \
		src/parser/expander_utils.c \
		src/parser/expander.c \
		src/parser/handle_errors.c \
		src/parser/handle_redirections.c \
		src/parser/handle_signal.c \
		src/parser/here_doc.c \
		src/parser/lexer_utils.c \
		src/parser/lexer.c \
		src/parser/memory_utils.c \
		src/parser/remove_quotes.c \
		src/parser/retokenizer.c \
		src/executor_helpers.c \
		src/executor_pipeline.c \
		src/executor_utils.c \
		src/executor.c \
		src/main.c \
		src/path.c \
		src/utils.c

OBJ = $(SRC:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a


all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re test
