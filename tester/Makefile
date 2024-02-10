# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nfour <nfour@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/12 16:33:38 by nfour             #+#    #+#              #
#    Updated: 2024/02/08 17:26:36 by nfour            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=libft_malloc_test

# max test is TEST4 == 32 all combined dlaf = 64 - 1
MAX_FLAG=63

REAL_TEST=malloc_test

LAST_TEST=test5

TEST=./test.sh

all:	${NAME}

${NAME}:
		@echo " \033[5;36m ----- Compiling malloc tester...  ----- \033[0m"
		@gcc -D USE_LIBFT_MALLOC src/test_implementation.c ../libft_malloc.so -o ${NAME} 2> /dev/null
		@gcc src/test_implementation.c -o ${REAL_TEST} 2> /dev/null

test:	${NAME}
		./${NAME} ${MAX_FLAG}
		./${REAL_TEST} ${MAX_FLAG}

test0:	${NAME}
		${TEST} 1 1

test1:	${NAME}
		${TEST} 2 2

test2:	${NAME}
		${TEST} 4 4

test3:	${NAME}
		${TEST} 8 8

test3b:	${NAME}
		${TEST} 16 16

test4:	${NAME}
		${TEST} 32 32

test5:	${NAME}
		@gcc src/test5.c ../libft_malloc.so -o test5 2> /dev/null && echo test5: && ./${LAST_TEST}

clean:
		@rm -rf ${NAME} ${REAL_TEST} ${LAST_TEST}
		@echo "\033[7;33m -----  Cleaning done  ----- \033[0m"

fclean:	clean
		@rm -rf ${NAME} ${REAL_TEST} ${LAST_TEST}

re:		fclean all

.PHONY:		all clean fclean re