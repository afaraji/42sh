/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:18:59 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:19:28 by awali-al         ###   ########.fr       */
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

void	setup_pipe_parent(t_job *j, pid_t pid, int *infile, int outfile)
{
	if (g_var.shell_is_interactive)
	{
		if (!j->pgid)
			j->pgid = pid;
		setpgid(pid, j->pgid);
	}
	if (*infile != j->fd_in)
		close(*infile);
	if (outfile != j->fd_out)
		close(outfile);
}

int		get_pipe(int mypipe[2], t_process *p, int fd_out)
{
	int	outfile;

	if (p->next)
	{
		system_calls("pipe", pipe(mypipe), -1);
		outfile = mypipe[1];
	}
	else
		outfile = fd_out;
	return (outfile);
}
