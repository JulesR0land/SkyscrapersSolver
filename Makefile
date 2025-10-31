.PHONY: all $(NAME) clean fclean re debug re_debug

SRC			=	main.c						\
				src/skyscrapers_solver.c	\
				src/skyscrapers_lines.c		\
				src/skyscrapers_cells.c

NAME		=	skyscrapers

CC			=	gcc

CFLAGS		=	-Wall -Wextra
CPPFLAGS	=	-I include/

OBJ			=	$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

debug: CFLAGS += -DDEBUG -g3 -O0 -fno-omit-frame-pointer
debug: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

re_debug: fclean debug
