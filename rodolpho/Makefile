SRCS = main.c ft_readline.c navigate_history.c miniparser.c \
		error.c resolve_path.c env_utils.c builtins_env_echo_pwd_unset.c \
		signal_handler.c expansion.c clear_memory.c builtin_exit.c \
		env_setup.c termcaps_termios.c cmdline_buffer.c cmdline_buffer_2.c execute_line.c \
		pipeline.c redirections.c run_builtin.c execute_binary.c \
		builtin_cd.c builtin_export.c

SRC = $(addprefix src/,$(SRCS))
OBJ = $(SRC:.c=.o)
LIBFT = ../libft/libft.a
INCLUDE = -I. -I../libft
CC = gcc
FLAGS = -Wall -Wextra -Werror  -g # debug flag

all:		minishell

minishell:	$(LIBFT) $(OBJ)	
			$(CC) $(FLAGS) $(OBJ) -lft -L../libft -ltermcap -o $@

OBJ_TEST = $(OBJ) src/main4test.o
test:		$(LIBFT) $(OBJ_TEST)
			$(CC) $(FLAGS) $(OBJ_TEST) -lft -L../libft -o $@

$(LIBFT):	
			$(MAKE) all clean -C ../libft/

%.o:		%.c
			$(CC) $(FLAGS) -c $(INCLUDE) $< -o $@

clean:
			rm -f $(OBJ)

fclean:		clean
			rm -f $(NAME) $(LIBFT)

re:			fclean all 
