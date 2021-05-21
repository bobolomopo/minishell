SRCS = main.c ft_readline.c navigate_history.c miniparser.c execute.c \
		error.c  launch_command.c pipeline.c  resolve_path.c env.c builtins.c \
		signal_handler.c ft_realloc.c
SRC = $(addprefix src/,$(SRCS))
OBJ = $(SRC:.c=.o)
LIBFT = ../libft/libft.a
INCLUDE = -I. -I../libft
CC = gcc
FLAGS = -Wall -Wextra -Werror 

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
