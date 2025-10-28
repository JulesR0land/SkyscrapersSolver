.PHONY: all $(NAME) clean fclean re run-tests

SRC			=	main.c						\
				src/skyscrapers_solver.c

NAME		=	skyscrapers

CC			=	gcc

CFLAGS		=	-Wall -Wextra

CPPFLAGS	=	-I include/

OBJ			=	$(SRC:.c=.o)

all:
	make $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

debug: CFLAGS += -DDEBUG -g
debug: all


clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

re_debug: fclean debug