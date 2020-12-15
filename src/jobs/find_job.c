/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_job.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 18:18:31 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:25:03 by awali-al         ###   ########.fr       */
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

t_job	*find_job_ppid(pid_t pid)
{
	t_job		*j;
	t_process	*p;

	j = g_var.job;
	while (j)
	{
		p = j->first_process;
		while (p)
		{
			if (p->pid == pid)
				return (j);
			p = p->next;
		}
		j = j->next;
	}
	return (NULL);
}

t_job	*find_job(pid_t pgid)
{
	t_job	*j;

	j = g_var.job;
	while (j)
	{
		if (j->pgid == pgid)
			return (j);
		j = j->next;
	}
	return (find_job_ppid(pgid));
}

int		find_job_and_update(pid_t pid, int status)
{
	t_job		*j;
	t_process	*p;

	if ((j = find_job(pid)) == NULL)
	{
		ft_print(STDERR, "ERROR pid [%d] not in any job.\n", pid);
		return (1);
	}
	j->notified = 0;
	p = j->first_process;
	while (p)
	{
		if (p->pid == pid)
		{
			p->status = status;
			if (WIFSTOPPED(status))
				p->stopped = 1;
			else if (WIFEXITED(status) || WIFSIGNALED(status))
				p->completed = 1;
		}
		p = p->next;
	}
	return (0);
}

pid_t	find_last_bg_job(void)
{
	t_job	*j;
	pid_t	tmp;

	j = g_var.job;
	tmp = 0;
	while (j)
	{
		if (j->index != 0 && j->pgid != g_current_job &&
													j->pgid != g_previous_job)
			tmp = j->pgid;
		j = j->next;
	}
	return (tmp);
}
