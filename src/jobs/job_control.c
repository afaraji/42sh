/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:12:20 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:35:24 by awali-al         ###   ########.fr       */
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

extern FILE	*tty;

void	grouped_job(t_and_or *cmd)
{
	int		pid;
	t_job	*job;

	system_calls("fork", (pid = fork()), -1);
	if (pid == 0)
	{
		pid = getpid();
		setpgid(pid, pid);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);
		exit(execute(cmd, pid));
	}
	else
	{
		setpgid(pid, pid);
		job = get_job_list(cmd);
		job->pgid = pid;
		job->first_process->pid = pid;
		add_job(job);
		put_job_in_background(job, 0);
	}
}

void	launch_process(t_process *p, t_job *j, int foreground, int io[2])
{
	char	*cmd_path;

	if (g_var.shell_is_interactive)
		set_for_interactive(j, foreground);
	if (io[1] != STDIN)
	{
		dup2(io[1], STDIN);
		close(io[1]);
	}
	if (io[0] != STDOUT)
	{
		dup2(io[0], STDOUT);
		close(io[0]);
	}
	if (do_simple_cmd(p->cmd) || p->argv == NULL)
		exit(1);
	p->env = env_to_tab(g_var.var, 0);
	if (is_builtin(p->argv[0]))
		exit(builtins(p->argv[0], p->argv, p->env));
	if (!(cmd_path = get_cmdpath(p->argv[0])))
		exit(127);
	execve(cmd_path, p->argv, p->env);
	ft_print(STDERR, "shell: permission denied: %s\n", p->argv[0]);
	exit(126);
}

void	setup_pipe(t_job *j, t_process *p, int *infile, int foreground)
{
	int		mypipe[2];
	int		outfile;
	pid_t	pid;

	outfile = get_pipe(mypipe, p, j->fd_out);
	system_calls("fork", (pid = fork()), -1);
	if (pid == 0)
	{
		if (*infile != mypipe[0])
			close(mypipe[0]);
		if (outfile != mypipe[1])
			close(mypipe[1]);
		mypipe[0] = outfile;
		mypipe[1] = *infile;
		launch_process(p, j, foreground, mypipe);
	}
	else
	{
		p->pid = pid;
		setup_pipe_parent(j, pid, infile, outfile);
		*infile = mypipe[0];
	}
}

int		launch_job(t_job *j, int foreground)
{
	t_process	*p;
	int			infile;

	if ((infile = exec_no_fork(j->cmd->ast, j->first_process->argv,
														!foreground)) != -42)
	{
		free_job(&j);
		return (infile);
	}
	infile = j->fd_in;
	p = j->first_process;
	while (p)
	{
		setup_pipe(j, p, &infile, foreground);
		p = p->next;
	}
	add_job(j);
	if (!g_var.shell_is_interactive)
		return (wait_for_job(j));
	else if (foreground)
		return (put_job_in_foreground(j, 0));
	else
		return (put_job_in_background(j, 0));
}

int		job_control(t_and_or *cmd, int bg)
{
	t_job		*job;
	int			dp;
	int			ret;

	if (tty == NULL)// to be deleted
		tty = fopen("/dev/ttys003", "w");
	ret = 0;
	if (bg && cmd->next)
	{
		grouped_job(cmd);
		return (ret);
	}
	while (cmd)
	{
		dp = cmd->dependent;
		if (!dp || (dp == 1 && !ret) || (dp == 2 && ret))
		{
			job = get_job(cmd);
			ret = launch_job(job, !bg);
			exit_status(ret << 8);
		}
		cmd = cmd->next;
	}
	return (ret);
}
