/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_report.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:06:53 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:27:01 by awali-al         ###   ########.fr       */
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

int		report_completed_job_2(t_job *j, t_process *p)
{
	char	c;
	int		ret;

	ret = 0;
	c = (j->pgid == g_current_job) ? '+' : ' ';
	c = (j->pgid == g_previous_job) ? '-' : c;
	if (WIFEXITED(p->status) && WEXITSTATUS(p->status) == 0)
		ft_print(STDOUT, "[%d]%c  Done\t\t%s\n", j->index, c, j->command);
	else if (WIFEXITED(p->status) && WEXITSTATUS(p->status) != 0)
		ft_print(STDOUT, "[%d]%c  Exit %d\t\t%s\n", j->index, c,
											WEXITSTATUS(p->status), j->command);
	else if (WIFSIGNALED(p->status))
	{
		ret = WTERMSIG(p->status);
		ft_print(STDOUT, "[%d]%c  %s: %d\t\t%s\n", j->index, c,
											ft_strsignal(ret), ret, j->command);
	}
	return (ret);
}

int		report_comleted_job(t_job *j, t_process *p, int fg)
{
	int		ret;

	ret = 0;
	if (j->index == 0 || fg)
	{
		if (WIFEXITED(p->status))
			ret = WEXITSTATUS(p->status);
		else
		{
			ret = WTERMSIG(p->status);
			if (ret != SIGINT)
				ft_print(STDOUT, "%s: %d\n", ft_strsignal(ret), ret);
			if (ret == SIGINT)
				ft_print(STDOUT, "\n");
		}
	}
	else
	{
		ret = report_completed_job_2(j, p);
	}
	if (remove_job(j) == 0)
		ft_print(STDERR, "JOBS: job not found.\n");
	return (ret);
}

int		report_stopped_job(t_job *j, t_process *p)
{
	int	ret;

	ret = 0;
	j->index = (j->index == 0) ? get_new_index() : j->index;
	give_current_job(j->pgid);
	p = j->first_process;
	while (p)
	{
		if (p->stopped == 1)
		{
			ret = WSTOPSIG(p->status);
			break ;
		}
		p = p->next;
	}
	ft_print(STDOUT, "[%d]+  %s\t\t%s\n", j->index, ft_strsignal(ret),
																	j->command);
	j->notified = 1;
	return (ret);
}

int		report_to_user(t_job *j, int fg)
{
	t_process	*p;
	int			ret;

	ret = 0;
	if (j->notified)
		return (0);
	p = j->first_process;
	while (p->next)
		p = p->next;
	if (job_is_completed(j))
	{
		ret = report_comleted_job(j, p, fg);
		return (ret);
	}
	if (job_is_stopped_completed(j))
	{
		ret = report_stopped_job(j, p);
		return (ret);
	}
	return (1);
}

void	notify_user(void)
{
	t_job	*j;

	j = g_var.job;
	while (j)
	{
		report_to_user(j, 0);
		j = j->next;
	}
}
