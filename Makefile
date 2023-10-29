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

SRCS	=	src/malloc.c \
			src/block.c \
			src/page_gestion.c \
			src/show_alloc_mem.c \
			src/utils.c \
			src/printf_fd/ft_intfunction.c \
			src/printf_fd/ft_printf.c \
			src/printf_fd/ft_put_and_count.c \

MAIN	= 	src/main.c \

OBJS = $(SRCS:.c=.o)

RM	= rm -f

HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

NAME	= libft_malloc_$(HOSTTYPE).so

LIBFT	= libft/libft.a

all:		${NAME}

%.o : %.c
	@$(CC) ${CFLAGS} -fPIC -c $< -o $@

${NAME}:	$(OBJS)
			@echo " \033[5;36m ----- Compiling malloc project...  ----- \033[0m\n"
			@$(CC) ${CFLAGS} -fPIC -shared -o $(NAME) $(OBJS)
			@echo "\033[7;32m -----  Compiling malloc done  ----- \033[0m\n"
			@ln -sf ${NAME} libft_malloc.so

clean:
			@echo "\033[7;31m\n -----  Cleaning all objects...  ----- \033[0m\n"
			@${RM} ${OBJS}
			@echo "\033[7;33m -----  Cleaning done  ----- \033[0m\n"

test : all
			@$(CC) $(CFLAGS) -o main_test $(SRCS) $(MAIN)
			@./main_test
			@rm main_test

testv : all
			@$(CC) $(CFLAGS) -o main_test $(SRCS) $(MAIN)
			@valgrind ./main_test
			@rm main_test

fclean:		clean
			@${RM} ${NAME} libft_malloc.so

re:			fclean all


.PHONY:		all clean fclean re bonus
