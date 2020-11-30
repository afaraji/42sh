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

	If the shell wishes to run the child in the background, then no waitpid( ) is
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

FILE	*tty;

int		update_proc(pid_t pid, int status, int bg);

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
	ft_print(2, "killed func: %s: [%d]\n", ft_strsignal(sig), sig);
	return (sig);
}

int		stopped_by(int sig)
{
	ft_print(2, "stopped func: %s: [%d]\n", ft_strsignal(sig), sig);
	return (sig);
}

// int		exec_ast_new(t_pipe_seq *cmd, int pgid)
// {
// 	int	child;
// 	int	pfd[2];

// 	pgid = (pgid == 0) ? getpid() : pgid;
// 	if (cmd->right)
// 	{
// 		pipe(pfd);
// 		if ((child = fork()) < 0)
// 		{
// 			perror("failed fork: ");
// 			exit(1);
// 		}
// 		if (child == 0)
// 		{
// 			setpgid(child, pgid);
// 			close(pfd[1]);
// 			dup2(pfd[0], STDIN);
// 			exec_ast_new(cmd->right, pgid);
// 		}
// 		else
// 		{
// 			close(pfd[0]);
// 			dup2(pfd[1], STDOUT);
// 			exit(exec_simple_cmd(cmd->left));
// 		}
// 	}
// 	exit(exec_simple_cmd(cmd->left));

// }

int	wait_cmd(t_pipe_seq *cmd)
{
	int	ret;

	while (cmd)
	{
		if (waitpid(-1, &ret, WUNTRACED | WCONTINUED) == -1)
		{
			perror("wait pid error: ");
		}
		cmd = cmd->right;
	}
	return (WEXITSTATUS(ret));
}

int		exec_ast_new(t_pipe_seq *cmd)
{
	t_pipe_seq	*tmp;
	int			pfd[2];
	int			infd;
	int			outfd;
	pid_t		child;

	infd = STDIN;
	tmp = cmd;
	while (tmp)
	{
		if (tmp->right)
		{
			system_calls("pipe", pipe(pfd), -1);
			outfd = pfd[1];
		}
		else
			outfd = STDOUT;
		child = system_calls("fork", fork(), -1);
		if (child == 0)
		{
			exec_simple_cmd(cmd->left);
		}
		if (infd != STDIN)
			close(infd);
		if (outfd != STDOUT)
			close(outfd);
		infd = pfd[0];
		tmp = tmp->right;
	}
	return (wait_cmd(cmd));
}

int		job_control(t_and_or *cmd, int bg)
{
	int	dp;
	int	ret;
	int	pgid;
tty = fopen("/dev/tty002", "w");
	while (cmd)
	{
		ret = 0;
		dp = cmd->dependent;
		if (!dp || (dp == 1 && !ret) || (dp == 2 && ret))
		{
			pgid = fork(); //dont fork if (cmd->ast->right == NULL && cmd->ast-left == builtin)
			if (pgid == 0)
			{
				setpgid(getpid(), getpid());
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				signal(SIGTSTP, SIG_DFL);
				signal(SIGTTIN, SIG_DFL);
				signal(SIGTTOU, SIG_DFL);
				signal(SIGCHLD, SIG_DFL);
				ft_set_attr(1);
				ret = exec_ast_new(cmd->ast);//exit !!!
			}
			else if (pgid < 0)
			{
				perror("fork failed: ");
				exit(1);
			}
			else
			{
				setpgid(pgid, pgid);
				tcsetpgrp(STDIN, pgid);
				int tmp;
				tmp = waitpid(pgid, &ret, WUNTRACED | WCONTINUED);
				fprintf(tty, "---------[%d]-[%d]--------\n", tmp, ret);
				exit_status(ret);// or ret = WEXITSTATUS(ret);
				// if (WIFEXITED(ret))
				// 	return (WEXITSTATUS(ret));
				// if (WIFSIGNALED(ret))
				// 	return (killed_by(WTERMSIG(ret)));
				// if (WIFSTOPPED(ret))
				// 	return (stopped_by(WSTOPSIG(ret)));
			}
		}
		cmd = cmd->next;
	}
	tcsetpgrp(STDIN, g_var.proc->ppid);
	ft_set_attr(0);
	(void)bg;
	return (ret);//or return (WEXITSTATUS(ret));
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
	t_proc	*p;

	if ((p = get_proc(pid)))
	{
		ft_print(STDOUT, "%s\n", p->str);
	}
	kill(pid, SIGSTOP);
	int ret = tcsetpgrp(STDIN, pid);// dont forget return value !!!
	ft_set_attr(1);
	kill(pid, SIGCONT);	// return value !!
	if ((ret = waitpid(pid, &status, WUNTRACED | WCONTINUED)) < 0)
	{
		perror("error at waitpid:");
		return (-2);
	}
	if (WIFCONTINUED(status))
	{
		// ret = tcsetpgrp(STDIN, pid);// dont forget return value !!!
		// ft_set_attr(1);
		update_proc(pid, status, 0);
		if (waitpid(pid, &status, WUNTRACED | WCONTINUED) < 0)
		{
			perror("error at waitpid:");
			return (-2);
		}
	}
	tcsetpgrp (STDIN, getpid());
	ft_set_attr(0);
	update_proc(pid, status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (killed_by(WTERMSIG(status)));
	if (WIFSTOPPED(status))
		return (stopped_by(WSTOPSIG(status)));
	return (0);
}

int		putjob_background(pid_t pid)
{
	int		status;
	t_proc	*p;

	status = 0;
	kill(pid, SIGCONT);
	pid = waitpid(pid, &status, WNOHANG | WUNTRACED);
	update_proc(pid, status, 0);
	if ((p = get_proc(pid)))
	{
		ft_print(STDOUT, "--->[%d]%c %s &\n", p->index, p->c, p->str);
	}
	return (0);
}

void	delet_proc(pid_t pid);

int		update_proc(pid_t pid, int status, int bg)
{
	t_proc	*p;

	p = g_var.proc;
	while (p)
	{
		if (p->ppid == pid)
			break ;
		p = p->next;
	}
	if ((WIFEXITED(status) || WIFSIGNALED(status)) && p)
	{
		// print new state ?
		if (WIFEXITED(status))
			ft_print(STDOUT, "[%d]%c  Done\t\t%s\n", p->index, p->c, p->str);
		else if (WTERMSIG(status) != 2)
			ft_print(STDOUT, "[%d]%c  Killed: %d\t\t%s\n", p->index, p->c, WTERMSIG(status), p->str);
			ft_print(STDOUT, "%s: %d\n", ft_strsignal(WTERMSIG(status)), WTERMSIG(status));
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
			p = add_proc(pid, 2);
		}
		ft_print(STDOUT, "\n[%d]%c  Stopped\t\t%s\n", p->index, p->c, p->str);
		return (1);
	}
	else if (WIFSIGNALED(status) && WTERMSIG(status) != 2)
	{
		ft_print(STDOUT, "%s: %d\n", ft_strsignal(WTERMSIG(status)), WTERMSIG(status));
	}
	else if (status == 0 && bg)//new job
	{
		p = add_proc(pid, 0);
		ft_print(STDOUT, "\n[%d] %d\n", p->index, p->ppid);
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
		// impliment here jobs argvs
		i++;
	}
	p = (g_var.proc->next);
	// printf("jobs list:[%d][%d]\n", g_var.proc->index, p->index);
	while (p)
	{
		ft_print(STDOUT,"[%d]%c\t", p->index, p->c);
		if (p->status == 0)
			ft_print(STDOUT,"running\t\t%s\n", p->str);
		else if (p->status == 1)
			ft_print(STDOUT,"done\t\t%s\n", p->str);
		else if (p->status == 2)
			ft_print(STDOUT,"stopped\t\t%s\n", p->str);
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
