CC = gcc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address
LDFLAGS = -L ./lib/libft -lft
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
HEADERS = $(wildcard src/.h)
NAME = ft_nm
INC = -I ./lib/libft

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(LDFLAGS) $(INC)  

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) $(INC)  

libs:
	$(MAKE) -C lib/libft

all: $(NAME)

clean: 
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)


re: fclean all


.PHONY: all clean fclean re
