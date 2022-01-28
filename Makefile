CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
LDFLAGS =
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
HEADERS = $(wildcard src/.h)
NAME = ft_nm
INC =

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(LDFLAGS) $(INC)  

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) $(INC)  

all: $(NAME)

clean: 
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)


re: fclean all


.PHONY: all clean fclean re
