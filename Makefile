CC = gcc
CFLAGS = -Wall -Wextra -Werror 
LDFLAGS = -L ./lib/libft -lft
SRC = $(wildcard src/*.c)
OBJ = $(SRC:%.c=%.o)
HEADERS = $(wildcard src/*.h)
NAME = ft_nm
INC = -I ./lib/libft
LIBFT=lib/libft/libft.a

ifdef DEBUG
	CFLAGS += -g -fsanitize=address
endif

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@ $(LDFLAGS) $(INC)

$(NAME): $(HEADERS) $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS) $(INC)

$(LIBFT):
	$(MAKE) -C lib/libft

all: $(NAME)

clean:
	$(RM) $(OBJ)
	$(MAKE) clean -C lib/libft

fclean: clean
	$(RM) $(NAME)
	$(MAKE) fclean -C lib/libft


re: fclean all

test: all
	#python3 test/main.py
	./ft_nm ./ft_nm


.PHONY: all clean fclean re test
