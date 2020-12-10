NAME=libft.a
SRC= $(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@ar rc $(NAME) $(OBJ)

%.0: %c
	@gcc -Wall -Wextra -Werror -c $< pt.h

.PHONY: clean mrproper

clean:
	@rm -rf *.o

fclean: clean
	@rm -rf $(NAME)

re: fclean $(NAME)
