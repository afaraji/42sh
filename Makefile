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

NAME = 42sh

SRC_SHL = ./src/main.c ./src/list_to_tab.c ./src/my_printf.c \
	./src/verify_type.c ./src/ft_replaceword.c

FILE_AST = add_redirect.c add_tokens.c alias.c alias_ast.c ast.c dollar_sub.c\
	get_pipe_sec.c get_splited_and_or.c get_suffix.c here_doc.c lexer.c		\
	tilde_sub.c

FILE_AUTOCMPLT = auto_complete.c auto_complete_2.c cmd_completion_1.c		\
	f_d_completion_1.c f_d_completion_3.c variables_completion.c 			\
	auto_complete_1.c cmd_completion.c f_d_completion.c f_d_completion_2.c	\
	print_completion.c

FILE_BUILTINS = builtins.c change_pwd.c export.c ft_cd_1.c ft_cd_2.c		\
	ft_cd_old.c  ft_echo.c  setenv.c  unalias.c unsetenv.c ft_fc.c

FILE_EXEC = do_assignement.c do_sufix_prefix.c exec.c exec_nofork.c			\
	exit_status.c get_cmdargs.c get_cmdpath.c io_redirect.c io_redirect_aggr.c\

FILE_FREE = ft_free.c ft_free_ast.c ft_free_g_var.c ft_strsplit_2.c

FILE_HIST = history_search.c manage_history.c navigate_history.c			\
	navigate_history_2.c

FILE_READLINE = copy.c get_next_line.c get_unprintable_char.c go_left.c go_up.c\
	init_shell.c left_select.c manage_line.c manage_terminal.c move_curs.c	\
	readline.c trim.c cut.c get_printable_char.c go_down.c go_right.c		\
	home_end.c join_line.c line_util.c manage_newline.c move_by_word.c past.c\
	right_select.c

FILE_PARSE = append_and_verify.c ft_tokenize.c join_tokens.c reserved_words.c\
	tokens_translate.c verify_tokens.c

FILE_JOBS = proc_manage.c signal.c jobs.c

FILE_SEARCH = back_search_1.c back_search_2.c

FILE_EXPANSION = expansion_arithmetic_utilities.c expansion_para.c\
				expansion_para_utilities.c expansions.c\
				expansions_arithmetic.c expansions_deprecated.c\
				hash_percent_expans.c logical_operation.c utilities_exp.c

SRC_AST = $(foreach file,$(FILE_AST), ./src/ast/$(file))

SRC_AUTOCMPLT = $(foreach file,$(FILE_AUTOCMPLT), ./src/autocomplete/$(file))

SRC_BUILTINS = $(foreach file,$(FILE_BUILTINS), ./src/builtins/$(file))

SRC_EXEC = $(foreach file,$(FILE_EXEC), ./src/exec/$(file))

SRC_FREE = $(foreach file,$(FILE_FREE), ./src/ft_free/$(file))

SRC_HIST = $(foreach file,$(FILE_HIST), ./src/history/$(file))

SRC_READLINE = $(foreach file,$(FILE_READLINE), ./src/readline/$(file))

SRC_PARSE = $(foreach file,$(FILE_PARSE), ./src/parse/$(file))

SRC_JOBS = $(foreach file,$(FILE_JOBS), ./src/jobs/$(file))

SRC_SEARCH = $(foreach file,$(FILE_SEARCH), ./src/incremental_search/$(file))

SRC_EXPANSION = $(foreach file,$(FILE_EXPANSION), ./src/expansions/$(file))

LIBFTA = ./libft/libft.a

CFLAGS = -Wall -Werror -Wextra

.SILENT:

OBJ_SHL = $(SRC_SHL:.c=.o)

OBJ_AST = $(SRC_AST:.c=.o)

OBJ_AUTOCMPLT = $(SRC_AUTOCMPLT:.c=.o)

OBJ_BUILTINS = $(SRC_BUILTINS:.c=.o)

OBJ_EXEC = $(SRC_EXEC:.c=.o)

OBJ_FREE = $(SRC_FREE:.c=.o)

OBJ_HIST = $(SRC_HIST:.c=.o)

OBJ_JOBS = $(SRC_JOBS:.c=.o)

OBJ_PARSE = $(SRC_PARSE:.c=.o)

OBJ_SEARCH = $(SRC_SEARCH:.c=.o)

OBJ_READLINE = $(SRC_READLINE:.c=.o)

OBJ_EXPANSIONS = $(SRC_EXPANSION:.c=.o)

all : msg $(NAME)

$(NAME) : $(OBJ_SHL) $(OBJ_AST) $(OBJ_AUTOCMPLT) $(OBJ_BUILTINS) $(OBJ_JOBS)\
		$(OBJ_EXEC) $(OBJ_FREE) $(OBJ_HIST) $(OBJ_PARSE) $(OBJ_READLINE) \
		$(OBJ_SEARCH) $(OBJ_EXPANSIONS)
		$(MAKE) -C ./libft
	printf "linking OBJ files... "
	gcc $(FLAGS) $(OBJ_SHL) $(LIBFTA) $(OBJ_AST) $(OBJ_AUTOCMPLT) $(OBJ_JOBS)\
		$(OBJ_BUILTINS) $(OBJ_EXEC) $(OBJ_FREE) $(OBJ_HIST) $(OBJ_PARSE) \
		$(OBJ_SEARCH) $(OBJ_READLINE) $(OBJ_EXPANSIONS) -ltermcap -o $(NAME)
	echo "done"

clean :
	printf "removing OBJ files ./src/\n"
	/bin/rm -f $(OBJ_SHL) $(OBJ_AST) $(OBJ_AUTOCMPLT) $(OBJ_BUILTINS)		\
		$(OBJ_EXEC) $(OBJ_FREE) $(OBJ_HIST) $(OBJ_PARSE) $(OBJ_READLINE)	\
		$(OBJ_JOBS) $(OBJ_SEARCH) $(OBJ_EXPANSIONS)
	$(MAKE) -C ./libft/ clean

fclean : clean
	printf "removing executable\n"
	/bin/rm -f $(NAME)
#	/bin/rm -f /tmp/.myshell_history
	$(MAKE) -C ./libft/ fclean

re : fclean all

msg :
	echo "compiling OBJ files with $(CFLAGS)"
