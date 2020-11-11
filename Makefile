# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/11 23:19:46 by afaraji           #+#    #+#              #
#    Updated: 2020/11/11 23:28:15 by afaraji          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh

SRC_SHL = ./src/list_to_tab.c ./src/my_printf.c ./src/signal.c ./src/main.c	\
	./src/proc_manage.c ./src/verify_type.c

SRC_AST =
SRC_AUTOCMPLT =
SRC_BUILTINS =
SRC_EXEC =
SRC_FREE =
SRC_HIST =
SRC_READLINE =
SRC_PARSE =

LIBFTA = ./libft/libft.a

SRC_LF = ./libft/ft_bzero.c ./libft/ft_isalnum.c ./libft/ft_isalpha.c		\
	./libft/ft_isdigit.c ./libft/ft_memalloc.c ./libft/ft_putstr.c			\
	./libft/ft_strcat.c ./libft/ft_strchr.c ./libft/ft_strcmp.c				\
	./libft/ft_strcpy.c ./libft/ft_strdup.c ./libft/ft_strjoin.c			\
	./libft/ft_strlen.c ./libft/ft_strnew.c	./libft/ft_strsplit.c			\
	./libft/ft_strsub.c

CFLAGS = -Wall -Werror -Wextra

OBJ_SHL = $(SRC_SHL:.c=.o)

OBJ_LF = $(SRC_LF:.c=.o)

all : $(NAME)

$(NAME) : $(OBJ_SHL) $(OBJ_LF)
	@$(MAKE) -C ./libft
	@gcc $(FLAGS) $(OBJ_SHL) $(LIBFTA) -ltermcap -o $(NAME)

clean :
	@printf "removing OBJ files ./src/\n"
	@/bin/rm -f $(OBJ_SHL)
	@$(MAKE) -C ./libft/ clean

fclean : clean
	@printf "removing executable\n"
	@/bin/rm -f $(NAME)
#	@/bin/rm -f /tmp/.myshell_history
	@$(MAKE) -C ./libft/ fclean

re : fclean all
