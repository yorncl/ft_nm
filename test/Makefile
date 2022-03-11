CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, bin/%.out, $(SRC)) \
      $(patsubst src/%.c, bin/%.nolink.out, $(SRC)) \

bin/%.out: src/%.c
	$(CC) $^ -o $@ 
bin/%.nolink.out: src/%.c
	$(CC) -c $^ -o $@ 

create_dir:
	mkdir -p ./bin

all: create_dir $(OBJ)

clean: 
	rm -rf ./bin
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all


.PHONY: all clean fclean re test
