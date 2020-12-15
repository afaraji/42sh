/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 20:44:09 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/15 19:43:05 by awali-al         ###   ########.fr       */
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

pid_t	g_current_job = 0;
pid_t	g_previous_job = 0;
FILE	*tty = NULL;

void	job_print(t_job *j, int l)
{
	char	c;
	int		sig;

	c = (j->pgid == g_current_job) ? '+' : ' ';
	c = (j->pgid == g_previous_job) ? '-' : c;
	sig = j->first_process->status;
	sig = WIFEXITED(sig) ? WEXITSTATUS(sig) : sig;
	sig = WIFSIGNALED(sig) ? WTERMSIG(sig) : sig;
	sig = WIFSTOPPED(sig) ? WSTOPSIG(sig) : sig;
	ft_print(STDOUT, "[%d]%c  ", j->index, c);
	if (l)
		ft_print(STDOUT, "%d %s: %d\t\t%s\n", j->pgid, ft_strsignal(sig), sig,
																	j->command);
	else
	{
		if (job_is_stopped_completed(j) == 0)
			ft_print(STDOUT, "running\t\t%s\n", j->command);
		else if (job_is_completed(j))
			ft_print(STDOUT, "done\t\t%s\n", j->command);
		else if (job_is_stopped_completed(j))
			ft_print(STDOUT, "stopped\t\t%s\n", j->command);
	}
}

int		ft_jobs_av(char **av)
{
	t_job	*j;
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	while (av[i])
	{
		if ((j = selected_job(av[i])) == NULL)
		{
			i++;
			ret = 1;
			continue ;
		}
		job_print(j, 0);
		i++;
	}
	return (ret);
}

int		ft_jobs_(char **av)
{
	t_job	*j;
	char	c;

	if (av && av[0])
		return (ft_jobs_av(av));
	j = g_var.job;
	while (j)
	{
		c = (j->pgid == g_current_job) ? '+' : ' ';
		c = (j->pgid == g_previous_job) ? '-' : c;
		ft_print(STDOUT, "[%d]%c\t", j->index, c);
		if (job_is_stopped_completed(j) == 0)
			ft_print(STDOUT, "running\t\t%s\n", j->command);
		else if (job_is_completed(j))
			ft_print(STDOUT, "done\t\t%s\n", j->command);
		else if (job_is_stopped_completed(j))
			ft_print(STDOUT, "stopped\t\t%s\n", j->command);
		j = j->next;
	}
	return (0);
}

int		ft_jobs_opt(char **av, int opt)
{
	int		i;
	int		ret;
	t_job	*j;

	i = 0;
	ret = 0;
	if (av && av[0])
	{
		while (av[i])
		{
			if ((j = selected_job(av[i])))
				(opt == 1) ? job_print(j, 1) : ft_print(1, "%d\n", j->pgid);
			else
				ret = 1;
			i++;
		}
		return (ret);
	}
	j = g_var.job;
	while (j)
	{
		(opt == 1) ? job_print(j, 1) : ft_print(STDOUT, "%d\n", j->pgid);
		j = j->next;
	}
	return (0);
}

int		ft_jobs(char **av)
{
	int		i;
	int		opt;

	opt = get_opt(av, &i);
	if (opt < 0)
	{
		ft_print(STDERR, "shell: jobs: -");
		ft_putchar_fd(-opt, STDERR);
		ft_print(STDERR, ": invalid option.\n");
		return (1);
	}
	if (opt == 1 || opt == 2)
	{
		return (ft_jobs_opt(&av[i], opt));
	}
	return (ft_jobs_(&av[i]));
}

// sleep x --> ^Z --> bg
// fix builtins arg management
// test leaks && sigfaults cases in get_cmdargs
// remove tty
