NAME = ft_printf.a

SRC = 	ft_intfunction.c \
		ft_printf.c \
		ft_put_and_count.c \

OBJ = $(SRC:.c=.o)

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

all : $(NAME)

bonus : $(BOBJ) $(OBJ)
		@ar rcs $(NAME) $^

%.o : %.c
		@$(CC) ${CFLAGS} -c $< -o $@

$(NAME): $(OBJ)
		@ar rcs -o $(NAME) $(OBJ)

clean :
		@rm -f $(OBJ) $(BOBJ)

fclean : clean
		@rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re bonus