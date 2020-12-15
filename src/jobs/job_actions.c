/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_actions.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:05:05 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:25:26 by awali-al         ###   ########.fr       */
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

int		update_job(t_job *j, pid_t pid, int status)
{
	t_process *p;

	p = j->first_process;
	while (p)
	{
		if (p->pid == pid)
		{
			j->notified = 0;
			p->status = status;
			if (WIFSTOPPED(status))
				p->stopped = 1;
			else if (WIFEXITED(status) || WIFSIGNALED(status))
				p->completed = 1;
			else if (WIFCONTINUED(status))
			{
				p->stopped = 0;
				p->completed = 0;
			}
			return (1);
		}
		p = p->next;
	}
	return (0);
}

void	add_job(t_job *j)
{
	t_job	*tmp;

	if (g_var.job == NULL)
		g_var.job = j;
	else
	{
		tmp = g_var.job;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = j;
	}
}

void	free_job(t_job **j)
{
	t_process	*p;
	t_process	*p2;

	if (!j || !*j)
		return ;
	ft_strdel(&((*j)->command));
	p = (*j)->first_process;
	while (p)
	{
		p2 = p;
		p = p->next;
		free_tab(p2->argv);
		free_tab(p2->env);
		ft_strdel(&(p2->command));
		free(p2);
		p2 = NULL;
	}
	free(*j);
	*j = NULL;
}

int		remove_job(t_job *job)
{
	t_job	*current;
	t_job	*prec;

	current = g_var.job;
	prec = NULL;
	while (current)
	{
		if (current->pgid == job->pgid)
		{
			if (prec)
				prec->next = current->next;
			else
				g_var.job = current->next;
			remove_current_job(job->pgid);
			free_job(&job);
			return (1);
		}
		prec = current;
		current = current->next;
	}
	return (0);
}

void	remove_current_job(pid_t pgid)
{
	if (g_current_job == pgid)
	{
		g_current_job = g_previous_job;
		g_previous_job = find_last_bg_job();
	}
	else if (g_previous_job == pgid)
	{
		g_previous_job = find_last_bg_job();
	}
}
