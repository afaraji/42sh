/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completed_job.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:28:05 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:29:25 by awali-al         ###   ########.fr       */
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

/*
** Return true if all processes in the job have stopped or completed.
*/

int		job_is_stopped_completed(t_job *j)
{
	t_process *p;

	p = j->first_process;
	while (p)
	{
		if (!p->completed && !p->stopped)
			return (0);
		p = p->next;
	}
	return (1);
}

/*
** Return true if all processes in the job have completed.
*/

int		job_is_completed(t_job *j)
{
	t_process *p;

	p = j->first_process;
	while (p)
	{
		if (!p->completed)
			return (0);
		p = p->next;
	}
	return (1);
}

void	give_current_job(pid_t pgid)
{
	if (g_current_job == pgid)
		return ;
	g_previous_job = g_current_job;
	g_current_job = pgid;
}

int		wait_for_job(t_job *j)
{
	pid_t		pid;
	int			status;

	while (job_is_stopped_completed(j) == 0)
	{
		pid = waitpid(-1, &status, WUNTRACED | WCONTINUED);
		if (pid == -1)
		{
			system_calls("waitpid: ", pid, -1);
		}
		if (update_job(j, pid, status) == 0)
		{
			find_job_and_update(pid, status);
		}
	}
	status = report_to_user(j, 1);
	return (status);
}

void	set_for_interactive(t_job *j, int foreground)
{
	pid_t	pid;

	pid = getpid();
	if (j->pgid == 0)
		j->pgid = pid;
	setpgid(pid, j->pgid);
	if (foreground)
	{
		tcsetpgrp(STDIN, j->pgid);
		ft_set_attr(1);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);
}
