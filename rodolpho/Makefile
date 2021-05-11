SRCS = main.c resolve_path.c
SRC = $(addprefix src/,$(SRCS))
OBJ = $(SRC:.c=.o)
LIBFT = ../libft/libft.a
INCLUDE = -I. -I../libft
CC = gcc
FLAGS = -Wall -Wextra -Werror
NAME = minishell

all:		$(NAME)

$(NAME):	 $(OBJ) $(LIBFT)
			$(CC) $(FLAGS) $(OBJ) -lft -L../libft -o $@

$(LIBFT):	
			$(MAKE) all clean -C ../libft/

%.o:		%.c
			$(CC) $(FLAGS) -c $(INCLUDE) $< -o $@

clean:
			rm -f $(OBJ)

fclean:		clean
			rm -f $(NAME)

re:			fclean all 