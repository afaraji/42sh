/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 17:49:31 by afaraji           #+#    #+#             */
/*   Updated: 2020/11/03 17:49:48 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

char	*last_in_hist(void)
{
	t_hist	*node;

	node = g_var.history;
	while (node->next)
		node = node->next;
	if (node->hist_str)
		return (ft_strdup(node->hist_str));
	return (ft_strdup(""));
}

char	give_current()
{
	t_proc	*p;

	p = (g_var.proc)->next;
	while (p)
	{
		if (p->c == '-')
			p->c = ' ';
		else if (p->c == '+')
			p->c = '-';
		p = p->next;
	}
	return ('+');
}

t_proc	*add_proc(pid_t pid, int done, int status)
{
	t_proc	*node;

	node = g_var.proc;
	while (node->next && node->next->ppid != pid)
		node = node->next;
	if (node->next == NULL)
	{
		node->next = (t_proc *)malloc(sizeof(t_proc));
		node->next->ppid = pid;
		node->next->index = node->index + 1;
		node->next->str = last_in_hist();
		node->next->next = NULL;
	}
	node->next->done = done;
	node->next->status = status;
	node->next->c = give_current();
	ft_putstr("--> add proc func[");
	ft_putnbr(node->next->index);
	ft_putstr("] ");
	ft_putnbr(pid);
	ft_putstr("\n");
	return (node->next);
}

t_proc	*get_proc_by_index(int index)
{
	t_proc	*p;

	p = g_var.proc->next;
	while (p)
	{
		if (p->index == index)
			return (p);
		p = p->next;
	}
	return (NULL);
}

void	find_new_previous(int big_i)
{
	t_proc	*p;

	while (big_i)
	{
		p = get_proc_by_index(big_i);
		if (p->c != '+')
		{
			p->c = '-';
			return ;
		}
		big_i--;
	}
}

void	find_new_current(char c)
{
	t_proc	*p;
	int		big_i;

	p = g_var.proc;
	big_i = 0;
	while (p->next)
		p = p->next;
	if ((big_i = p->index) == 0 || (c != '+' && c != '-'))
		return ;
	p = g_var.proc->next;
	if (c == '+')
	{
		while (p)
		{
			if (p->c == '-')
			{
				p->c = '+';
				break ;
			}
			p = p->next;
		}
		find_new_previous(big_i);
	}
	else// c == '-'
		find_new_previous(big_i);
}

void	delet_proc(pid_t pid)
{
	t_proc	*node;
	t_proc	*old_node;

	node = g_var.proc;
	old_node = NULL;
	while (node)
	{
		if (pid == node->ppid)
		{
			ft_strdel(&(node->str));
			old_node->next = node->next;
			find_new_current(node->c);
			free(node);
			break ;
		}
		old_node = node;
		node = node->next;
	}
}


int		find_job_and_update(pid_t pid, int status);
void	notify_user(void);



void	bg_jobs(void)
{
	// t_proc	*proc;
	int		pid;
	int		status;

	while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
	{
		printf("debug_msg:updating proc [%d]-status[%d]\n", pid, status);
		find_job_and_update(pid, status);
	}
	notify_user();
}
// was working on updating jobs (still ned to give current and previous valuse...)
