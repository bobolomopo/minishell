# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/06 17:43:34 by jandre            #+#    #+#              #
#    Updated: 2021/05/06 17:43:34 by jandre           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell.a
FLAG = -Wall -Wextra -Werror
OPTION = -c
SRC = ft_get_next_line.c ft_isin.c ft_only_str.c ft_refree.c ft_split.c \
	ft_strcpy.c ft_strjoin.c ft_strlen.c ft_strnew.c ft_substr.c ft_tokenize.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@ar rc $(NAME) $(OBJ)

%.o:%.c
	@gcc $(FLAG) $(OPTION) $< -o $@

clean:
	@rm -rf $(OBJ) $(BOBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re