/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 20:44:09 by afaraji           #+#    #+#             */
/*   Updated: 2020/11/14 20:44:14 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

/*
****** ref: https://www.cs.uleth.ca/~holzmann/C/system/pipeforkexec.html *******
****** code: https://www.gnu.org/software/libc/manual/html_node/Launching-Jobs.html
The shell maintains a table of jobs. Before every prompt for a command, the shell does a:

	waitpid(-1, &status, WNOHANG | WUNTRACED)
	to check if any of its children have changed status. Status changes are reported
	to the user and table of jobs is updated.
	The shell executes a pipeline list by forking off a child to handle the list.
	This child immediately makes itself a process group leader by changing its PGID
	to its own PID. To avoid race conditions the shell should also issue the command
	to change the process group of the child. Then the child and any of its
	descendants can be signaled as a unit. No descendent will leave this group
	(excepting perhaps a new interactive subshell).

	If the shell wishes to run this child in the foreground, then it will change the
	terminal's process group to that of the child and then do a waitpid( ) on the
	child's pid. The child now has control of the terminal and the shell disappears
	as far as the user is concerned. A ^Z, if not caught by the child, will stop the
	child (and the group) and the shell's waitpid( ) will return. The shell
	determines that a stop was sent to the child by looking at the status value. At
	this point the child still has control of the terminal. The shell uses an
	tcsetpgrp( ) to get the terminal back but here is a tricky detail: since the
	shell is not in the terminal's group the tcsetpgrp( ) will be sent a SIGTTOU
	signal. In order to prevent being stopped by this signal, the shell must make
	sure that it has ignored (SIG_IGN) signal SIGTTOU before doing the tcsetpgrp( ).
	After recording the child as a suspended job in the shell's table of jobs and
	resetting SIGTTOU, the shell proceeds probably prompting for the next command.

*	If the shell wishes to run the child in the background, then no waitpid( ) is
	done and the terminal's process group remains that of the shell. The entire
	child process group will be sent a SIGTTIN or SIGTTOU and become suspended if
	any descendent attempts I/O on the terminal. If no attempt is made the child
	runs to completion and turns into a zombie until the shell finally does a
	waitpid( ) for it.

	If the shell receives a fg command referring to a background child, a SIGSTSP is
	sent to the child's process group, the terminal is given to the child via an
	tcsetpgrp( ), a SIGCONT is sent to the child's process group, and then the shell
	does a waitpid( ) for this child.

	If a bg command is received for a currently suspended job, then a SIGCONT is
	sent to the child's process group and the shell proceeds without doing a
	waitpid( ).
*/

FILE	*ttp;
FILE	*ttc;
extern int errno;

int		update_proc(pid_t pid, int status);

char	*ft_getsigstr1_12(int sig)
{
	static char *str[13];
	if (!str[0])
	{
		str[0] = "done";
		str[1] = "hangup";
		str[2] = "Interrupt";
		str[3] = "Quit";
		str[4] = "Illegal instruction";
		str[5] = "trace trap";
		str[6] = "abort";
		str[7] = "EMT trap";
		str[8] = "floating point exception";
		str[9] = "killed";
		str[10] = "bus error";
		str[11] = "segmentation fault";
		str[12] = "Bad system call";
	}
	if (sig >= 0 && sig <= 12)
		return (str[sig]);
	return (NULL);
}
char	*ft_getsigstr13_31(int sig)
{
	static char *str[20];
	if (!str[0])
	{
		str[0] = "broken pipe";	//13
		str[1] = "Alarm clock";	//14
		str[2] = "terminated";	//15
		str[4] = "stoped";	//17
		str[5] = "stoped";	//18
		str[8] = "stoped";	//21
		str[9] = "stoped";	//22
		str[11] = "Cputime limit exceeded";	//24
		str[12] = "Filesize limit exceeded"; //25
		str[13] = "Virtual timer expired"; //26
		str[14] = "Profiling timer expired"; //27
		str[17] = "User defined signal 1"; //30
		str[18] = "User defined signal 2"; //31
	}
	if (sig >= 13 && sig <= 31)
		return (str[sig - 13]);
	return (NULL);
}
char	*ft_strsignal(int sig)
{
	if (sig <= 12)
		return (ft_getsigstr1_12(sig));
	return (ft_getsigstr13_31(sig));
}

int		killed_by(int sig)
{
	fprintf(ttp, "killed func: %s: [%d]\n", ft_strsignal(sig), sig);
	return (sig);
}

int		stopped_by(int sig)
{
	fprintf(ttp, "stopped func: %s: [%d]\n", ft_strsignal(sig), sig);
	return (sig);
}

int		exec_ast_fg(t_pipe_seq *cmd)
{
	int			child;
	int			status;

	fprintf(ttp, "-1--- exec_ast_fg:[is a buitin] ---\n");
	if ((status = exec_no_fork(cmd, 0)) != -42)
	{
		fprintf(ttp, "-2--- exec_ast_fg:[is a buitin] --ret[%d]-\n", status);
		return (status << 8);
	}
	fprintf(ttp, "-3--- exec_ast_fg:[not a buitin..forking] ---\n");
	status = 0;
	child = fork();
	if (child == 0)
	{
		fprintf(ttc, "-a--- child ---[%d|%d] - [%d]\n", getpid(), getpgrp(), child);
		child = getpid();
		setpgid(child, child);
		int ret = tcsetpgrp (STDIN, child); // dont forget return value !!!
		fprintf(ttc, "-b--- child ---[%d|%d] - [%d] ---ret [%d]\n", getpid(), getpgrp(), child, ret);
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
		ft_set_attr(1);
		fprintf(ttc, "-c--- child ---[%d]\nexec_pip -->> ", getpid());
		exec_pipe(cmd);
	}
	else
	{
		int ret;
		fprintf(ttp, "------1----\n");
		ret = setpgid(child, child);
		fprintf(ttp, "------2----[%d]\n", ret);
		tcsetpgrp (STDIN, child); // dont forget return value !!!
		signal (SIGTTOU, SIG_IGN);
		signal (SIGTTIN, SIG_IGN);
		fprintf(ttp, "------3----[%d]\n", ret);
		ret = waitpid(child, &status, WUNTRACED | WCONTINUED);
		if (ret < 0)
		{
			fprintf(ttp, "------4----[%d] - {err: %s}\n", ret, strerror(errno));
		}
		fprintf(ttp, "------5----[%d]\n", ret);
		fprintf(ttp, "--6---- returned from wait [%x]\n", status);
		update_proc(child, status);
		ret = tcsetpgrp (STDIN, getpid()); // dont forget return value !!!
		ft_set_attr(0);
		exit_status(status);
		fprintf(ttp, "--7---- ret_tcset[%d]\n", ret);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		if (WIFSIGNALED(status))
			return (killed_by(WTERMSIG(status)));
		if (WIFSTOPPED(status))
			return (stopped_by(WSTOPSIG(status)));
	}
	return (status);
}

int		execute_fg(t_and_or *cmd)
{
	int dp;
	int ret;

	ret = 0;
	while (cmd)
	{
		dp = cmd->dependent;
		if (!dp || (dp == 1 && !ret) || (dp == 2 && ret))
		{
			fprintf(ttp, "---- executing_ast_fg ---\n");
			ret = exec_ast_fg(cmd->ast);
			exit_status(ret);
		}
		cmd = cmd->next;
	}
	return (ret);
}

int		exec_ast_bg(t_pipe_seq *cmd)
{
	int			child;
	int			status;

	if ((status = exec_no_fork(cmd, 0)) != -42)
		return (status << 8);
	status = 0;
	child = fork();
	if (child == 0)
	{
		exec_pipe(cmd);
	}
	else
	{
		if (waitpid(child, &status, WUNTRACED | WCONTINUED) < 0)
			return (-2);
		update_proc(child, status);
		exit_status(status);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		if (WIFSIGNALED(status))
			return (killed_by(WTERMSIG(status)));
		if (WIFSTOPPED(status))
			return (stopped_by(WSTOPSIG(status)));
	}
	return (status);
}

int		execute_bg(t_and_or *cmd)
{
	int dp;
	int ret;

	ret = 0;
	while (cmd)
	{
		dp = cmd->dependent;
		if (!dp || (dp == 1 && !ret) || (dp == 2 && ret))
		{
			ret = exec_ast_bg(cmd->ast);
			exit_status(ret);
		}
		cmd = cmd->next;
	}
	exit(ret);
}

int		job_control(t_and_or *cmd, int bg)
{
	int		status;
	pid_t	pid;
	// return (execute(cmd, bg)); //uncoment to go back to old execution
	//************************************************************************
	ttp = fopen("/dev/ttys005", "w");
	ttc = fopen("/dev/ttys006", "w");
	fprintf(ttp, "\033[H\033[2J");
	fprintf(ttc, "\033[H\033[2J");
	fprintf(ttp, "++++++++++++ debuging ++++++++++++\n");
	fprintf(ttc, "++++++++++++ debuging ++++++++++++\n");
	//************************************************************************
	pid = waitpid(-1, &status, WNOHANG | WUNTRACED);
	if (pid > 0)
	{
		fprintf(ttp, "pid[%d] has changed state to [%x]\n", pid, status);
		update_proc(pid, status);
	}
	if (bg)
	{
		fprintf(ttp, "---- lunching job in BG ---\n");
		if ((pid = fork()) < 0)
			return (-1);//print error and exit instad ?!!
		if (pid == 0)
		{
			pid = setsid();
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);
			signal(SIGTTIN, SIG_DFL);
			signal(SIGTTOU, SIG_DFL);
			signal(SIGCHLD, SIG_DFL);
			fprintf(ttc, "---- execute_BG ---\n");
			execute_bg(cmd);
		}
		else
		{
			fprintf(ttp, "---adding to job list\n");
			add_proc(pid, 0);
		}
	}
	else
	{
		fprintf(ttp, "---- lunching job in FG ---parent[%d|%d]\n", getpid(), getpgrp());
		execute_fg(cmd);
	}
	fprintf(ttp, "---parent done returning (0)\n");
	return (0);
}

t_proc	*get_proc(pid_t pid)
{
	t_proc	*p;

	p = (g_var.proc)->next;
	while (p)
	{
		if (p->ppid == pid)
			return (p);
		p = p->next;
	}
	return (p);
}

int		putjob_forground(pid_t pid)
{
	int		status;
	t_proc	*p;int ret;// i was here why he dosent waitpid

	if ((p = get_proc(pid)))
	{
		ft_print(STDOUT, "%s\n", p->str);
	}
	delet_proc(pid);
	ft_set_attr(1);
	tcsetpgrp(STDIN, pid);// dont forget return value !!!
	kill(pid, SIGCONT);	// return value !!
	if ((ret = waitpid(pid, &status, WUNTRACED | WCONTINUED)) < 0)
	{
		perror("error at waitpid:");
		return (-2);
	}
	fprintf (ttp, "proc done ?? ---> pid[%d]-ret[%d]\n", pid, ret);
	// update_proc(pid, status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (killed_by(WTERMSIG(status)));
	if (WIFSTOPPED(status))
		return (stopped_by(WSTOPSIG(status)));
	tcsetpgrp (STDIN, getpid());
	ft_set_attr(0);
	return (0);
}

int		putjob_background(pid_t pid)
{
	int		status;
	t_proc	*p;

	status = 0;
	kill(pid, SIGCONT);
	pid = waitpid(pid, &status, WNOHANG | WUNTRACED);
	update_proc(pid, status);
	if ((p = get_proc(pid)))
	{
		ft_print(STDOUT, "[%d]%c %s &\n", p->index, p->c, p->str);
	}
	return (0);
}

void	delet_proc(pid_t pid);

int		update_proc(pid_t pid, int status)
{
	t_proc	*p;

	p = g_var.proc;
	fprintf(ttp, "-1--update proc---\n");
	while (p)
	{
		if (p->ppid == pid)
			break ;
		p = p->next;
	}
	fprintf(ttp, "-2--update proc---\n");
	if ((WIFEXITED(status) || WIFSIGNALED(status)) && p)
	{
		// print new state ?
		if (WIFEXITED(status))
			ft_print(STDOUT, "[%d]%c  Done\t\t%s\n", p->index, p->c, p->str);
		else
			ft_print(STDOUT, "[%d]%c  Killed: %d\t\t%s\n", p->index, p->c, WTERMSIG(status), p->str);
		delet_proc(pid);
	}
	else if (WIFSTOPPED(status))
	{
		if (p)
		{
			p->done = 0; // add running/done/stoped ?
			p->status = 2; // #define STOPPED x ?
		}
		else
		{
			add_proc(pid, 2);
			p = get_proc(pid);
		}
		ft_print(STDOUT, "\n[%d]%c  Stopped\t\t%s\n", p->index, p->c, p->str);
		return (1);
	}
	return (0);
}

int		get_opt(char **av, int *index)
{
	int		i;
	int		j;
	int		opt;

	i = 1;
	opt = 0;
	while (av[i] && (av[i][0] == '-' && av[i][1]))
	{
		j = 1;
		while (av[i][j])
		{
			if (av[i][j] == 'l')
				opt = 1;
			else if (av[i][j] == 'p')
				opt = 2;
			else
				return (-(av[i][j]));
			j++;
		}
		i++;
	}
	*index = i;
	return (opt);
}

pid_t	get_pid_str(char *s)
{
	t_proc	*p;
	pid_t	pid;
	int		len;

	len = ft_strlen(s);
	p = (g_var.proc)->next;
	pid = 0;
	if (*s == '?')
	{
		s++;
		while (p)
		{
			if (ft_strstr(p->str, s))
			{
				pid = p->ppid;
				break ;
			}
			p = p->next;
		}
	}
	else
	{
		while (p)
		{
			if (!ft_strncmp(p->str, s, len))
			{
				pid = p->ppid;
				break ;
			}
			p = p->next;
		}
	}
	if (pid == 0)
		ft_print(STDERR, "shell: job_util: %s: no such job.\n", s);
	return (pid);
}

pid_t	get_pid_n_plus_min(char c, char *s)
{
	t_proc	*p;
	int		index;

	p = (g_var.proc)->next;
	index = (s != NULL) ? ft_atoi(s) : -1;
	while (p)
	{
		if (p->c == c || (c == -1 && p->index == index))
			return (p->ppid);
		p = p->next;
	}
	ft_putstr_fd("shell: job_util: ", STDERR);
	if (c == '+')
		ft_putstr_fd("current", STDERR);
	else if (c == '-')
		ft_putstr_fd("previous", STDERR);
	else
		ft_putstr_fd(s, STDERR);
	ft_putstr_fd(": no such job.\n", STDERR);
	return (0);
}

int		ft_bg(char **av)
{
	pid_t	p;

	p = 0;
	if (av[1])
	{
		if (av[1][0] == '%')
		{
			if (av[1][1] == '+' || av[1][1] == '%')
				p = get_pid_n_plus_min('+', NULL);
			else if (av[1][1] == '-')
				p = get_pid_n_plus_min('-', NULL);
			else if (is_all_digits(&av[1][1]))
				p = get_pid_n_plus_min(-1, &(av[1][1]));
			else
				p = get_pid_str(&av[1][1]);
		}
		else
			p = get_pid_str(av[1]);
	}
	else
		p = get_pid_n_plus_min('+', NULL);
	if (p == 0)
		return (1);
	return (putjob_background(p));
}

int		ft_fg(char **av)
{
	pid_t	p;

	p = 0;
	if (av[1])
	{
		if (av[1][0] == '%')
		{
			if (av[1][1] == '+' || av[1][1] == '%')
				p = get_pid_n_plus_min('+', NULL);
			else if (av[1][1] == '-')
				p = get_pid_n_plus_min('-', NULL);
			else if (is_all_digits(&av[1][1]))
				p = get_pid_n_plus_min(-1, &(av[1][1]));
			else
				p = get_pid_str(&av[1][1]);
		}
		else
			p = get_pid_str(av[1]);
	}
	else
		p = get_pid_n_plus_min('+', NULL);
	if (p == 0)
		return (1);
	return (putjob_forground(p));
}

int		ft_jobs_(char **av)
{
	int		i;
	t_proc	*p;

	i = 0;
	while (av && av[i])
	{
		i++;
	}
	p = g_var.proc;
	p = p->next;
	printf("jobs list:\n");
	while (p)
	{
		printf("[%d]-[%d]\tdone[%d]\tstatus[%d]\t%s\n", p->index, p->ppid, p->done, p->status, p->str);
		p = p->next;
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
		return (2);
	}
	if (opt == 1)//l
	{
		ft_jobs_(&av[i]);
	}
	else if (opt == 2)//p
	{
		ft_jobs_(&av[i]);
	}
	else
	{
		ft_jobs_(&av[i]);
	}
	return (0);
}

// int		job_control(t_and_or *cmd, int bg)
// {
// 	int		status;
// 	pid_t	pid;

// 	pid = waitpid(-1, &status, WNOHANG | WUNTRACED);
// 	if (pid > 0)
// 	{
// 		printf("pid[%d] has changed state to [%x]\n", pid, status);
// 		update_proc(pid, status);
// 	}
// 	if ((pid = fork()) < 0)
// 		return (-1);//print error and exit instad ?!!
// 	if (pid == 0)
// 	{
// 		pid = setsid();
// 		if (!bg)
// 			tcsetpgrp (STDIN, pid); // dont forget return value !!!
// 		signal (SIGINT, SIG_DFL);
// 		signal (SIGQUIT, SIG_DFL);
// 		signal (SIGTSTP, SIG_DFL);
// 		signal (SIGTTIN, SIG_DFL);
// 		signal (SIGTTOU, SIG_DFL);
// 		signal (SIGCHLD, SIG_DFL);
// 		ft_set_attr(1);
// 		// exec cmd;
// 	}
// 	else
// 	{
// 		setpgid(pid, pid);
// 		if (!bg)
// 		{
// 			signal (SIGTTOU, SIG_IGN);
// 			signal (SIGTTIN, SIG_IGN);
// 			if (waitpid(pid, &status, WUNTRACED | WCONTINUED) < 0)
// 				return (-2);
// 			// The shell determines that a stop was sent to the child by looking at the status value
// 			update_proc(pid, status);
// 			if (WIFEXITED(status))
// 				return (WEXITSTATUS(status));
// 			if (WIFSIGNALED(status))
// 				return (killed_by(WTERMSIG(status)));
// 			if (WIFSTOPPED(status))
// 				return (stopped_by(WSTOPSIG(status)));
// 			tcsetpgrp (STDIN, getpid()); // dont forget return value !!!
// 			// add child as suspended process in process list
// 		}
// 		else
// 		{
// 			add_proc(pid);
// 			ft_set_attr(0);
// 		}
// 	}
// 	return (0);
// }
