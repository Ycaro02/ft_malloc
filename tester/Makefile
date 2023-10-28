# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nfour <nfour@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/12 16:33:38 by nfour             #+#    #+#              #
#    Updated: 2023/09/16 10:24:18 by nfour            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= gcc

CFLAGS	= -Wall -Wextra -Werror -g

SRCS	=	test.c\
			test1.c\
			test1_4.c\

OBJS = $(SRCS:.c=.o)

RM	= rm -f

HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

NAME	= test

all:		${NAME}
	echo $(HOSTTYPE)
%.o : %.c
	@$(CC) ${CFLAGS} -c $< -o $@

${NAME}:	$(OBJS)
			@echo " \033[5;36m ----- Compiling malloc project...  ----- \033[0m\n"
			@$(CC) ${CFLAGS} -fPIC -shared -o $(NAME) $(OBJS)
			@echo "\033[7;32m -----  Compiling malloc done  ----- \033[0m\n"
			ln -sf ${NAME} libft_malloc.so

clean:
			@echo "\033[7;31m\n -----  Cleaning all objects...  ----- \033[0m\n"
			@${RM} ${OBJS}
			@echo "\033[7;33m -----  Cleaning done  ----- \033[0m\n"

fclean:		clean
			@${RM} ${NAME} libft_malloc.so

re:			fclean all

.PHONY:		all clean fclean re bonus