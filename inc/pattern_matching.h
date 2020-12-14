/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:20:41 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/14 19:04:50 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PATTERN_MATCHING_H
# define PATTERN_MATCHING_H

# include "ttslist.h"
# include "../libft/libft.h"
# include "parse.h"
# include "expansion.h"
# include "dirent.h"
# include "sys/stat.h"
# include <stdio.h>
# define PRINTABLE_CHARS_BUFFER		256
# define MATCH_ONCE					1
# define MATCH_ZERO_OR_MORE			2
# define SITUATION_STATUS_NONE		0
# define SITUATION_STATUS_FAIL		-1
# define SITUATION_STATUS_SUCCESS	1

typedef struct		s_range_match
{
	unsigned char	except;
	unsigned char	error;
	int				match_count;
	char			characters[PRINTABLE_CHARS_BUFFER];
	int				buffer_size;
}					t_range_match;

int					evaluate_current_situation(t_list_node *range,
		char *to_match, int match_index);
unsigned char		range_matches_character(char current_char,
		t_range_match *current_range);
unsigned char		match_strings_to_range(char *to_match, t_list_node *range,
		int match_index);
void				add_character_to_range(t_range_match *range, char c);
int					find_bracket_end_index(char *expression, int index);
void				handle_range_characters(t_range_match *result,
		char *expression, int i, int escaped);
t_range_match		parse_bracket_range(char *expression);
t_range_match		parse_brackets(char *expression, int *index);
int					handle_opening_bracket(char *expression, int i,
		t_range_match *result);
t_list_head			get_pattern_to_match(char *expression, int *index, int i,
		int expression_size);
char				*extract_expression(char *initial_expression,
		int start_index, int *match_folders);
int					is_dir(char *path);
t_list_head			get_file_names(char *directory, unsigned char hidden);
int					merge_ttslist(t_list_head *list1, t_list_head *list2);
void				handle_empty_expression(t_list_head *past_result,
		t_list_head *current_result);
int					handle_first_time_expression(t_list_head *current_results,
		t_list_head *ranges, int match_folders, char first_char);
int					handle_normal_expression(t_list_head *current_results,
		t_list_head *ranges, t_list_head *past_results, int *params);
int					match_file_results(char *initial_expression,
		int start_index, t_list_head *past_results, t_list_head *final_result);
char				*unescaped_string(char *expression);
void				handle_star_mark(t_range_match *result);
void				handle_interrogation_mark(t_range_match *result);
char				**expand_pattern(char **argv);
char				**list_to_tab_oz(t_list_head *arg_list);
t_list_head			tab_to_list(char **argv);
t_list_head			pattern_matching(char *expression);
int					is_pattern(char *c);
char				*unescaped_string(char *expression);
void				swap(char **xp, char **yp);
void				match_file_results_free(char *ex, t_list_head *ranges,
						t_list_head *past_results, t_list_head *cr);
#endif
