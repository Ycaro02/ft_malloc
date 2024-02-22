# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nfour <<marvin@42.fr>>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/12 16:33:38 by nfour             #+#    #+#              #
#    Updated: 2024/02/21 10:10:38 by nfour            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= gcc

THREAD  =	-lpthread

CFLAGS	= -Wall -Wextra -Werror -g3

SRCS	=	src/malloc.c 						\
			src/block.c 						\
			src/page_gestion.c					\
			src/show_alloc_mem.c 				\
			src/utils.c 						\
			src/realloc.c						\
			src/free.c							\
			src/trace_alloc.c					\
			src/handle_env.c					\


LIBFT	= 	libft/libft.a

OBJS = $(SRCS:.c=.o)

RM	= rm -f

# Mandatory rule set hostype if it is empty or non-existant
# HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME	= libft_malloc_$(HOSTTYPE).so

LINK_NAME = libft_malloc.so

LIBFT	= libft/libft.a

# Replace malloc lib for valgrind read check
REPLACE_MALLOC_LIB	=	"--soname-synonyms=somalloc=${NAME}"

HELGRIND = "--tool=helgrind"

TEST	=	main_test

MAIN	= 	tester/src/main_test.c

CALL_TESTER="make -s -C tester"

all:		${NAME}

%.o : %.c
	@$(CC) ${CFLAGS} -fPIC -c $< -o $@

${NAME}:	$(OBJS)
			@echo " \033[5;36m ----- Compiling lib...  ----- \033[0m\n"
			@make -s -C libft
			@echo "\033[7;32m -----  Compiling lib done  ----- \033[0m\n"
			@echo " \033[5;36m ----- Compiling malloc project...  ----- \033[0m\n"
			@$(CC) ${CFLAGS} -fPIC -shared -o $(NAME) $(OBJS) ${THREAD} ${LIBFT}
			@echo "\033[7;32m -----  Compiling malloc done  ----- \033[0m"
			@ln -sf ${NAME} ${LINK_NAME}

rtest:
			@"${CALL_TESTER}" rtest

test :		${NAME}
			@"${CALL_TESTER}" test0
			@"${CALL_TESTER}" test1
			@"${CALL_TESTER}" test2
			@"${CALL_TESTER}" test3
			@"${CALL_TESTER}" test3b
			@"${CALL_TESTER}" test4
			@"${CALL_TESTER}" test5
			@"${CALL_TESTER}" fclean

testv :		${NAME}
			@$(CC) ${CFLAGS} -o $(TEST) ${MAIN} $(OBJS) ${LIBFT} ${THREAD}
			@echo "\033[7;32m -----  Compiling malloc test  ----- \033[0m"
			@valgrind ${REPLACE_MALLOC_LIB} ${HELGRIND} ./${TEST}
# @valgrind ${HELGRIND} ./${TEST}

clean:
			@echo "\033[7;31m\n -----  Cleaning all objects...  ----- \033[0m\n"
			@${RM} ${OBJS}
			@make -s -C libft clean
			@"${CALL_TESTER}" clean
			@echo "\033[7;33m -----  Cleaning done  ----- \033[0m\n"


fclean:		clean
			@make -s -C libft fclean
			@"${CALL_TESTER}" fclean
			@${RM} ${NAME} ${LINK_NAME}
			@${RM} ${TEST}

re:			fclean all


.PHONY:		all clean fclean re bonus
