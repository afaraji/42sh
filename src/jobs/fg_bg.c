/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg_bg.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:16:54 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:24:18 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"
#include "../../inc/jobs.h"

extern pid_t	g_current_job;
extern pid_t	g_previous_job;

t_job	*selected_job(char *s)
{
	t_job	*j;

	if (s[0] == '%')
	{
		if (s[1] == '+' || s[1] == '%')
			j = get_pid_n_plus_min('+', NULL);
		else if (s[1] == '-')
			j = get_pid_n_plus_min('-', NULL);
		else if (is_all_digits(&s[1]))
			j = get_pid_n_plus_min(-1, &(s[1]));
		else
			j = get_pid_str(&s[1]);
	}
	else
		j = get_pid_str(s);
	return (j);
}

int		ft_bg(char **av)
{
	char	c;
	t_job	*j;

	j = NULL;
	if (av[1])
	{
		j = selected_job(av[1]);
	}
	else
		j = get_pid_n_plus_min('+', NULL);
	if (j == 0)
		return (1);
	c = (j->pgid == g_current_job) ? '+' : ' ';
	c = (j->pgid == g_previous_job) ? '-' : c;
	printf("-1-> [%d]%c  %s &\n", j->index, c, j->command);
	return (put_job_in_background(j, 1));
}

int		ft_fg(char **av)
{
	t_job	*j;

	j = NULL;
	if (av[1])
	{
		j = selected_job(av[1]);
	}
	else
		j = get_pid_n_plus_min('+', NULL);
	if (j == NULL)
		return (1);
	ft_print(STDOUT, "%s\n", j->command);
	return (put_job_in_foreground(j, 1));
}

int		put_job_in_background(t_job *j, int cont)
{
	if (g_current_job == 0)
		g_current_job = j->pgid;
	else if (g_previous_job == 0)
		g_previous_job = j->pgid;
	j->notified = 0;
	j->index = (j->index == 0) ? get_new_index() : j->index;
	if (cont)
	{
		if (kill(-j->pgid, SIGCONT) < 0)
			system_calls("kill (SIGCONT)", 1, 1);
	}
	else
		ft_print(STDOUT, "[%d] %d\n", j->index, j->pgid);
	return (0);
}

int		put_job_in_foreground(t_job *j, int cont)
{
	int		ret;
	pid_t	pid;

	tcsetpgrp(STDIN, j->pgid);
	if (cont)
	{
		ft_set_attr(1);
		if (kill(-j->pgid, SIGCONT) < 0)
		{
			system_calls("kill (SIGCONT)", 1, 1);
		}
		pid = waitpid(-j->pgid, &ret, WUNTRACED | WCONTINUED);
		if (WIFCONTINUED(ret))
		{
			update_job(j, pid, ret);
		}
	}
	ret = wait_for_job(j);
	tcsetpgrp(STDIN, g_var.shell_pid);
	ft_set_attr(0);
	return (ret);
}
