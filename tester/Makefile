# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ycaro <ycaro@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/12 16:33:38 by nfour             #+#    #+#              #
#    Updated: 2024/02/21 16:10:43 by ycaro            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	libft_malloc_test

# LD_LIBRARY_PATH=.
# max test is TEST4 == 32 all combined dlaf = 64 - 1
MAX_FLAG	=	63

REAL_TEST	=	malloc_test

LAST_TEST	=	test5

TEST		=	./test.sh

CHECK_LIB	=	./check_lib.sh

MY_LIB		=	../libft/libft.a ../libft_malloc.so

RM			=	rm -rf


all:	${NAME}

${NAME}:
		@${CHECK_LIB}
		@echo " \033[5;36m ----- Compiling malloc tester...  ----- \033[0m"
		@gcc -D USE_LIBFT_MALLOC src/test_implementation.c ${MY_LIB} -o ${NAME} 2> /dev/null
		@gcc src/test_implementation.c -o ${REAL_TEST} 2> /dev/null

test:	${NAME}
		./${NAME} ${MAX_FLAG}
# ./${REAL_TEST} ${MAX_FLAG}

# flag for test:
# test0 	--> 1
# test1 	--> 2
# test2 	--> 4
# test3		--> 8
# test3_bis --> 16
# test4 	--> 32

test0:	${NAME}
		${TEST} 1 1

test1:	${NAME}
		${TEST} 2 2

test2:	${NAME}
		${TEST} 4 4

test3:	${NAME}
		./${NAME} 8
# ${TEST} 8 8

test3b:	${NAME}
		./${NAME} 16
# ${TEST} 16 16

test4:	${NAME}
		./${NAME} 32

test5:
		@gcc src/test5.c ../libft_malloc.so -o ${LAST_TEST} 2> /dev/null && ./${LAST_TEST}

rtest:	${NAME}
		@${CHECK_LIB}
		./correct.sh

clean:
		@${RM} ${NAME} ${REAL_TEST} ${LAST_TEST}

fclean:	clean
		@${RM} ${NAME} ${REAL_TEST} ${LAST_TEST} libft_malloc.so

re:		fclean all

.PHONY:		all clean fclean re