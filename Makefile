
CC=	gcc

NAME=	libcclosure.so

CFLAGS	+= -Wall -Wextra -I include
SRC=	src/array.c \
	src/closure.c \
	src/algorithm.c \
	src/make_copy.c \
	src/closure_algorithm.c \
	src/iterator.c \
	src/linked_list.c

OBJ=	$(SRC:.c=.o)


$(NAME):	$(OBJ)
	$(CC) -shared -o $(NAME) $(OBJ)

%.o:	%.c
	$(CC) $(CFLAGS) -fPIC -o $@ -c $<

all:	$(NAME)

clean:
	@rm -f $(OBJ)

fclean:	clean
	@rm -f $(NAME)

re:	fclean $(NAME) clean
