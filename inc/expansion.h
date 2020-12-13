/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 01:34:25 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/13 18:07:03 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H
# include "ft_21sh.h"
# include "exec.h"
# include "builtins.h"

# define PP		':'
# define PPR	'^'

void			ft_move_space(char **str);
char			*ft_iitoa(long long int n);
int				expans_arithmetic(char **argument, int start, int end);
int				hash_param(char **word);
int				param_is_set(char *var);
void			set_local_variable(char **word, int i);
int				operation(char **operate, char **word, int check);
int				normal_case(char **word);
int				sep_count(char **word);
int				expans_para(char **word);
int				expans_parameter(char **argument, int start, int end);
long long int	get_first_operand(char **str, int *err);
int				expans_arith(char **operation);
int				dollar_replace(char **argument, int i, int end);
int				tilde_to_replace(char **argument, int i, int end);
int				check_parenth_close(char **argument, int *end,
									int i, char c);
int				check_type(char **argument, int *end, int i);
int				juje_dolarate(char **argument, int index);
void			ft_expans_replace(char **argument, char *to_change,
									int start, int end);
int				expansions_dispatcher(char **argument);
char			*var_get_value(char *var, int typ);
char			*ft_strndup(const char *src, size_t n);
char			*get_trim_str(char *word, int *percent_pos, char c);
void			triming_end(char **word, char *trim, int percent_pos);
void			triming_start(char **word, char *trim, int hash_pos);
int				percent_para(char **word);
int				hash_separator_para(char **word);
char			*ft_strjoin_free(char *s1, char *s2, int p);
void			logical_operator(char *operator, char *str);
int				logical_do_op(long long int first_operand,
							long long int second_operand, char operator);
int				dollared(char *word);
void			clean_shities(char **word);
void			replace_2p(char **word);
int				expansions(t_list_token *cmd_token);
int				ft_strrstr(char *haystack, char *needle,
							size_t len, int lim);
int				next_sta(char *str);
int				find_and_replace(char **trim, char *val, int *i);
void			trim_replace(char **trim, char *val, int *i);
int				ques_dollar(char **argument);
int				ft_strstri(const char *haystack, const char *needle);
void			before_and_not_aft(char **trim, char *val, int i);
int				second_case_baf(char **trim, char *val, int i, int ns);
int				before_and_aft(char **trim, char *val, int i);
void			trim_end_replace(char **trim, char *val, int *i);
int				only_after(char **trim, char *val, int *i);
int				ilegal_do(char *word, char c);
int				norming_shiiiiiiiit(char **argument, char **word, int start);
void			nor_s(char **operate, char **word, int check);
int				ft_strchri(const char *s, int c);
int				exp_err(char *err);

#endif
