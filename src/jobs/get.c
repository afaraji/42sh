/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 18:12:24 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:53:42 by awali-al         ###   ########.fr       */
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

int			get_new_index(void)
{
	t_job	*node;
	int		index;

	node = g_var.job;
	index = 1;
	while (node)
	{
		index = (node->index >= index) ? node->index + 1 : index;
		node = node->next;
	}
	return (index);
}

t_process	*get_process(t_simple_cmd *cmd)
{
	t_process	*p;

	p = (t_process *)ft_malloc(sizeof(t_process));
	p->argv = get_arg_var_sub(cmd);
	p->env = NULL;
	p->pid = 0;
	p->completed = 0;
	p->stopped = 0;
	p->status = 0;
	p->cmd = cmd;
	p->command = simple_cmd_to_str(cmd);
	p->next = NULL;
	return (p);
}

t_process	*get_first_proc(t_pipe_seq *cmd)
{
	t_process	*head;
	t_process	*node;

	head = NULL;
	while (cmd)
	{
		if (!head)
		{
			head = get_process(cmd->left);
			node = head;
		}
		else
		{
			node->next = get_process(cmd->left);
			node = node->next;
		}
		cmd = cmd->right;
	}
	return (head);
}

t_job		*get_job(t_and_or *cmd)
{
	t_job	*job;

	job = (t_job *)ft_malloc(sizeof(t_job));
	job->cmd = cmd;
	job->command = ast_to_str(cmd->ast);
	job->first_process = get_first_proc(cmd->ast);
	job->pgid = 0;
	job->notified = 0;
	job->index = 0;
	job->fd_in = STDIN;
	job->fd_out = STDOUT;
	job->fd_err = STDERR;
	job->next = NULL;
	return (job);
}

t_job		*get_job_list(t_and_or *cmd)
{
	t_job	*job;

	job = (t_job *)ft_malloc(sizeof(t_job));
	job->cmd = cmd;
	job->command = and_or_to_str(cmd);
	job->first_process = get_first_proc(cmd->ast);
	job->pgid = 0;
	job->notified = 0;
	job->index = 0;
	job->fd_in = STDIN;
	job->fd_out = STDOUT;
	job->fd_err = STDERR;
	job->next = NULL;
	return (job);
}
