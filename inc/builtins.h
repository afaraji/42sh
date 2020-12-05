/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 16:49:32 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/05 18:14:24 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H
# include "ft_21sh.h"

# define FLAGS "lenrs"
# define FLGSIZE 6
# define S_OPT	0
# define L_OPT	1
# define N_OPT	2
# define R_OPT	3
# define E_OPT	4

int		get_next_line(const int fd, char **line);
int		ft_cd(char *flag, char **env);
char	*get_real_path(char *path);
int		ft_cd_1(char *flag, char **env);
int		ft_cd_old(char **env);
int		ft_cd_home(char **env);
int		ft_setenv(char **flag, char **env);
int		ft_unsetenv(char **flag);
int		ft_echo(char **flag);
int		change_pwd(char *str1, char *str2);
void	free_tab(char **flag);
int		verify_type(char *file);
int		file_or_dir_link(char **file);
int		verify_type_1(char **file, struct stat st);
void	print_env(char **env);
int		ft_export(char **flag, char **env);
void	ft_export_2(char *key, char *value);
int		get_key_value(char **key, char **value, char *flag);
int		ft_pdenied(char *flag);
char	*ft_get_ld(char *cwd, char *flag);
int		ft_fc(char **av);
int		ft_fc_1(char *f, char *l, int opt[5], char *e);
char	*ft_replaceword(char *s, char *old, char *new);
int		fc_add_to_file(t_hist *list, char *editor, int r);
int		fc_do_s(char **av, int i);
int		ft_editor(char *editor, char *file);
int		fc_print_list(t_hist *list, int n, int r);
t_hist	*get_fc_list(char *first_s, char *last_s, int l);
t_hist	*get_hist_node(int index);
t_hist	*get_fc_list_1(t_hist *start, t_hist *end, int reverse);
void	get_fc_list_2(t_hist *start, t_hist *end, t_hist **list, t_hist **node);
int		get_index_hist_last(char *s, int l, int first_index);
int		get_index_hist_first(char *s, int l);
void	get_index_hist_first_1(int *index, t_hist *head);
void	fc_history_add(char *s, int l);
char	*fc_history_remove(void);
int		get_last_hist(void);
int		get_str_index(char *s);
int		verify_index(int index);
int		get_opt_av(int opt[5], char **av, char **editor);
int		get_opt_av_1(int opt[5], char **av, char **editor, int *i);
int		get_opt_str(int opt[5], char *str);

#endif
