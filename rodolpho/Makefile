SRCS = main.c resolve_path.c
SRC = $(addprefix src/,$(SRCS))
OBJ = $(SRC:.c=.o)
LIBFT = ../libft/libft.a
INCLUDE = -I. -I../libft
CC = gcc
FLAGS = -Wall -Wextra -Werror -g
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
			rm -f $(NAME) $(LIBFT)

re:			fclean all 

pipes:		src/pipes.o $(LIBFT)
			$(CC) $(FLAGS) $< -lft -L../libft -o $@

OBJ_LC = src/launch_command.o src/resolve_path.o src/error.o
launch_command:	$(LIBFT) $(OBJ_LC)
			$(CC) $(FLAGS) $(OBJ_LC) -lft -L../libft -o $@