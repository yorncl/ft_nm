CC = gcc
CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address
LDFLAGS = -L ./lib/libft -lft
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
HEADERS = $(wildcard src/.h)
NAME = ft_nm
INC = -I ./lib/libft
LIBFT=lib/libft/libft.a

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(LDFLAGS) $(INC)  

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) $(INC)  

$(LIBFT):
	$(MAKE) -C lib/libft

all: $(NAME)

clean: 
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)


re: fclean all

test:
	python3 test/main.py


.PHONY: all clean fclean re test
