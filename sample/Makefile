
NAME=	sample

SRC=	main.c

CFLAGS += -Wall -Wextra -I ../include

LDFLAGS = -L.. -lcclosure

LDFLAGS += -Wl,-rpath=../	

OBJ=	$(SRC:.c=.o)


$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS)

all:	$(NAME)

clean:
	@rm -f $(OBJ)

fclean:	clean
	@rm -f $(NAME)

re:	fclean $(NAME) clean
