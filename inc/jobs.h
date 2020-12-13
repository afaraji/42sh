/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 20:45:09 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/13 16:23:51 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOBS_H
# define JOBS_H
# include "ft_21sh.h"

void		free_job(t_job **j);
char		*ft_getsigstr1_12(int sig);
char		*ft_getsigstr13_31(int sig);
char		*ft_strsignal(int sig);
char		*io_redirect_to_str(t_io_redirect *io);
char		*args_to_str2(t_cmd_suffix *suff, char *s);
char		*args_to_str(t_cmd_suffix *suff, t_cmd_prefix *pref);
char		*simple_cmd_to_str(t_simple_cmd *cmd);
char		*ast_to_str(t_pipe_seq *cmd);
char		*and_or_to_str(t_and_or *cmd);
int			get_new_index(void);
t_process	*get_process(t_simple_cmd *cmd);
t_process	*get_first_proc(t_pipe_seq *cmd);
t_job		*get_job(t_and_or *cmd);
t_job		*get_job_list(t_and_or *cmd);
int			job_is_stopped_completed(t_job *j);
int			job_is_completed(t_job *j);
int			update_job(t_job *j, pid_t pid, int status);
void		add_job(t_job *j);
void		free_job(t_job **j);
pid_t		find_last_bg_job(void);
void		give_current_job(pid_t pgid);
void		remove_current_job(pid_t pgid);
int			remove_job(t_job *job);
int			report_to_user(t_job *j, int fg);
void		notify_user(void);
t_job		*find_job_ppid(pid_t pid);
t_job		*find_job(pid_t pgid);
int			find_job_and_update(pid_t pid, int status);
int			wait_for_job(t_job *j);
int			put_job_in_background (t_job *j, int cont);
int			put_job_in_foreground (t_job *j, int cont);
void		launch_process (t_process *p, t_job *j, int foreground, int io[2]);
int			launch_job(t_job *j, int foreground);
void		grouped_job(t_and_or *cmd);
int			job_control(t_and_or *cmd, int bg);
int			get_opt(char **av, int *index);
t_job		*get_pid_str(char *s);
t_job		*get_pid_n_plus_min(char c, char *s);
int			ft_bg(char **av);
int			ft_fg(char **av);
int			ft_jobs_(char **av);
int			ft_jobs(char **av);
int			find_job_and_update(pid_t pid, int status);
void		notify_user(void);
void		bg_jobs(void);

#endif
