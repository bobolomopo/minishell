# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/06 16:30:05 by jandre            #+#    #+#              #
#    Updated: 2021/05/06 16:30:05 by jandre           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
FLAGS = -Wall -Wextra -Werror

all: minishell

utils:
	make -C utils/

minishell: utils
	$(CC) $(FLAGS) main.c utils/minishell.a -o $@

clean:
	make clean -C utils

fclean:
	make fclean -C utils
	rm -rf minishell

re: fclean
	make

.PHONY: checker push_swap clean fclean re utils