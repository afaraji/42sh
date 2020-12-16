# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/11 23:19:46 by afaraji           #+#    #+#              #
#    Updated: 2020/12/16 13:54:08 by awali-al         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 42sh

OBJS = objs

AST_PATH = ast

HEADER_FILES =	inc/ast.h inc/builtins.h inc/exec.h inc/expansion.h 		\
	inc/ft_21sh.h inc/ft_free.h inc/hash_table.h inc/jobs.h inc/parse.h		\
	inc/pattern_matching.h inc/readline.h inc/test.h inc/ttslist.h

SRC_SHL = ./objs/main.o ./objs/list_to_tab.o ./objs/my_printf.o \
	./objs/verify_type.o ./objs/ft_replaceword.o ./objs/list_to_tab_1.o

FILE_AST = add_redirect.o add_tokens.o alias.o alias_ast.o ast.o dollar_sub.o\
	get_pipe_sec.o get_splited_and_or.o get_suffix.o here_doc.o lexer.o		\
	tilde_sub.o cmd_name.o free_remove_quote.o

FILE_AUTOCMPLT = auto_complete.o auto_complete_2.o cmd_completion_1.o		\
	f_d_completion_1.o f_d_completion_3.o variables_completion.o 			\
	auto_complete_1.o cmd_completion.o f_d_completion.o f_d_completion_2.o	\
	print_completion.o

FILE_BUILTINS = builtins.o change_pwd.o export.o export_print.o ft_cd_1.o 	\
	ft_cd_2.o ft_cd_old.o  ft_echo.o  setenv.o setenv_1.o  unalias.o unsetenv.o		\
	hash.o test.o test_functions.o ft_type.o ft_fc.o ft_fc_1.o ft_fc_2.o 	\
	ft_fc_3.o ft_fc_4.o ch_prnt_dir.o ft_strshave.o

FILE_HASHTABLE = ft_str2del.o ft_strappend.o ft_strstich.o hash_add.o		\
	hash_chck.o hash_table.o alias_check.o

FILE_EXEC = do_assignement.o do_sufix_prefix.o exec.o exec_nofork.o			\
	exit_status.o get_cmdargs.o get_cmdpath.o io_redirect.o io_redirect_aggr.o\
	error_list.o

FILE_FREE = ft_free.o ft_free_ast.o ft_free_g_var.o ft_strsplit_2.o			\
	free_token_node.o

FILE_HIST = history_search.o manage_history.o navigate_history.o			\
	navigate_history_2.o history_sub.o

FILE_JOBS = proc_manage.o signal.o jobs.o completed_job.o fg_bg.o find_job.o \
	ft_strsignal.o get.o get_2.o job_actions.o job_control.o job_pipe.o \
	job_report.o to_str.o to_str_2.o

FILE_READLINE = copy.o get_next_line.o get_unprintable_char.o go_left.o go_up.o\
	init_shell.o left_select.o manage_line.o manage_terminal.o move_curs.o	\
	readline.o trim.o cut.o get_printable_char.o go_down.o go_right.o		\
	home_end.o join_line.o line_util.o manage_newline.o move_by_word.o past.o\
	right_select.o

FILE_PARSE = append_and_verify.o ft_tokenize.o join_tokens.o reserved_words.o\
	tokens_translate.o verify_tokens.o quot_close.o

FILE_SEARCH = back_search_1.o back_search_2.o

FILE_EXPANSION = expansion_arithmetic_utilities.o expansion_para.o			\
	expansion_para_utilities.o expansions.o expansions_arithmetic.o			\
	expansions_deprecated.o	hash_percent_expans.o logical_operation.o 		\
	utilities_exp.o hash_percent_utilities.o expansion_norm_helper.o		\
	has_percent_n.o has_percent_n2.o

FILE_PATTERN = pattern_matching.o pattern_matching_bracket_parser.o			\
	pattern_matching_evaluate_situation.o pattern_matching_handle_expression.o\
	pattern_matching_handle_range.o pattern_matching_handler.o 				\
	pattern_matching_match.o pattern_matching_util.o ttslist_constructors.o	\
	ttslist_delete.o ttslist_iterator.o ttslist_operations.o ttslist_purge.o\
	ttslist_tools.o pattern_matching_util_2.o

SRC_AST = $(foreach file,$(FILE_AST), ./objs/ast/$(file))

SRC_AUTOCMPLT = $(foreach file,$(FILE_AUTOCMPLT), ./objs/autocomplete/$(file))

SRC_BUILTINS = $(foreach file,$(FILE_BUILTINS), ./objs/builtins/$(file))

SRC_HASHTABLE = $(foreach file,$(FILE_HASHTABLE), ./objs/hashtable/$(file))

SRC_EXEC = $(foreach file,$(FILE_EXEC), ./objs/exec/$(file))

SRC_FREE = $(foreach file,$(FILE_FREE), ./objs/ft_free/$(file))

SRC_HIST = $(foreach file,$(FILE_HIST), ./objs/history/$(file))

SRC_READLINE = $(foreach file,$(FILE_READLINE), ./objs/readline/$(file))

SRC_PARSE = $(foreach file,$(FILE_PARSE), ./objs/parse/$(file))

SRC_JOBS = $(foreach file,$(FILE_JOBS), ./objs/jobs/$(file))

SRC_SEARCH = $(foreach file,$(FILE_SEARCH), ./objs/incremental_search/$(file))

SRC_EXPANSION = $(foreach file,$(FILE_EXPANSION), ./objs/expansions/$(file))

SRC_PATTERN = $(foreach file,$(FILE_PATTERN), ./objs/pattern_matching/$(file))

OBJ = $(SRC_AST) $(SRC_AUTOCMPLT) $(SRC_BUILTINS) $(SRC_HASHTABLE) $(SRC_EXEC) \
		$(SRC_FREE) $(SRC_HIST) $(SRC_READLINE) $(SRC_PARSE) $(SRC_JOBS) \
		$(SRC_SEARCH) $(SRC_EXPANSION) $(SRC_PATTERN) $(SRC_SHL)

LIBFTA = ./libft/libft.a

CFLAGS = -Wall -Werror -Wextra

$(OBJS)/%.o : ./src/%.c $(HEADER_FILES) | objsdir
	echo "make object $@"
	$(CC) $(CFLAGS) -c $< -o $@

CC=gcc

all : msg $(NAME)

$(NAME) : $(OBJ) $(HEADER_FILES)
	make -C ./libft
	printf "linking OBJ files... "
	$(CC) $(CFLAGS) $(OBJ) $(LIBFTA) -ltermcap -o $(NAME)
	echo "done"

clean :
	printf "removing OBJ files.\n"
	/bin/rm -fr $(OBJS)
	$(MAKE) -C ./libft/ clean

fclean : clean
	printf "removing executable\n"
	/bin/rm -f $(NAME)
#	/bin/rm -f /tmp/.myshell_history
	$(MAKE) -C ./libft/ fclean

re : fclean all

msg :
	echo "compiling OBJ files with $(CFLAGS)"

.SILENT:
objsdir :
	mkdir -p objs
	mkdir -p objs/ast
	mkdir -p objs/autocomplete
	mkdir -p objs/builtins
	mkdir -p objs/exec
	mkdir -p objs/expansions
	mkdir -p objs/ft_free
	mkdir -p objs/hashtable
	mkdir -p objs/history
	mkdir -p objs/incremental_search
	mkdir -p objs/jobs
	mkdir -p objs/parse
	mkdir -p objs/pattern_matching
	mkdir -p objs/readline
