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

SRCS	=	src/malloc.c 						\
			src/block.c 						\
			src/page_gestion.c					\
			src/show_alloc_mem.c 				\
			src/utils.c 						\
			src/realloc.c						\

MAIN	= 	src/main.c

PRINTF  =	printf_fd/ft_printf.a

TEST	=	main_test

OBJS = $(SRCS:.c=.o)

RM	= rm -f

HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

NAME	= libft_malloc_$(HOSTTYPE).so

LIBFT	= libft/libft.a

all:		${NAME}

%.o : %.c
	@$(CC) ${CFLAGS} -fPIC -c $< -o $@

${NAME}:	$(OBJS)
			@echo " \033[5;36m ----- Compiling printf lib...  ----- \033[0m\n"
			@make -s -C printf_fd
			@echo "\033[7;32m -----  Compiling printf done  ----- \033[0m\n"
			@echo " \033[5;36m ----- Compiling malloc project...  ----- \033[0m\n"
			@$(CC) ${CFLAGS} -fPIC -shared -o $(NAME) $(OBJS) ${PRINTF}
			@echo "\033[7;32m -----  Compiling malloc done  ----- \033[0m\n"
			@ln -sf ${NAME} libft_malloc.so

test :		all
			@${CC} ${CFLAGS} -o ${TEST} ${SRCS} ${MAIN} ${PRINTF}
			@./${TEST}

test0 :		all
			make -C tester test0

test1 :		all
			make -C tester test1

test2 :		all
			make -C tester test2
testv :
			@${CC} ${CFLAGS} -o ${TEST} ${SRCS} ${MAIN} ${PRINTF}
			@valgrind --soname-synonyms=somalloc=libft_malloc.so ./${TEST}

clean:
			@echo "\033[7;31m\n -----  Cleaning all objects...  ----- \033[0m\n"
			@${RM} ${OBJS}
			@make -s -C printf_fd clean
			@echo "\033[7;33m -----  Cleaning done  ----- \033[0m\n"


fclean:		clean
			@make -s -C printf_fd fclean
			@${RM} ${NAME} libft_malloc.so
			@${RM} ${TEST}

re:			fclean all


.PHONY:		all clean fclean re bonus
