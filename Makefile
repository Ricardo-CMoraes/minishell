NAME = minishell

CC = cc
CFLAGS = -g -Wall -Wextra -Ilibft -Iincludes

SRC = src/main.c \
		src/lexer.c \
		src/utils.c \
		src/expander.c \
		src/expander_utils.c \
		src/retokenizer.c \
		src/remove_quotes.c \
		src/build_commands.c \
		src/build_commands_utils.c \
		src/handle_redirections.c \
		src/builtins/builtins.c \
		src/builtins/fd_echo.c \
		src/builtins/fd_env.c \
		src/builtins/fd_pwd.c \
		src/builtins/fd_cd.c \
		src/builtins/fd_export.c \
		src/builtins/fd_unset.c \
		src/builtins/fd_exit.c \
		src/builtins/env_utils.c \
		src/executor.c \
		src/path.c \
		src/handle_errors.c \
		src/here_doc.c \
		src/copy_environment.c

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
