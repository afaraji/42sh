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

void	initShell(void)
{
	int shell_terminal;
	int	shell_is_interactive;

	shell_terminal = STDIN;
	shell_is_interactive = isatty (shell_terminal);
	if (shell_is_interactive)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGCHLD, SIG_IGN);
		if (setpgid (g_var.proc->ppid, g_var.proc->ppid) < 0)
		{
			perror ("Couldn't put the shell in its own process group: ");
			exit (1);
		}
		tcsetpgrp (shell_terminal, g_var.proc->ppid);
		if (ft_set_attr(0))
		{
			perror ("Couldn't set attributes: ");
			exit (1);
		}
	}
}

char	*suffix_to_str(t_cmd_suffix *suff)
{
	char	*str;
	char	*tmp;

	str = ft_strdup("");
	while (suff)
	{
		if (suff->word)
		{
			tmp = ft_strjoin(str, suff->word);
			ft_strdel(&str);
			str = tmp;
		}
		suff = suff->suffix;
	}
	return (str);
}

char	*ast_to_str(t_pipe_seq *cmd)
{
	char	*tmp;
	char	*suffix;
	char	*name;
	char	*str;
	char	*pipe;

	str = ft_strdup("");
	while (cmd)
	{
		name = (cmd->left->name) ? cmd->left->name : cmd->left->word;
		suffix = suffix_to_str(cmd->left->suffix);
		pipe = (cmd->right) ? "|" : "";
		tmp = ft_4strjoin(str, name, suffix, pipe);
		ft_strdel(&str);
		ft_strdel(&suffix);
		str = tmp;
		cmd = cmd->right;
	}
	return (str);
}

char	*and_or_to_str(t_and_or *cmd)
{
	char	*ast;
	char	*token;
	char	*str;
	char	*tmp;

	str = ft_strdup("");
	while (cmd)
	{
		ast = ast_to_str(cmd->ast);
		if (cmd->dependent)
			token = (cmd->dependent == 1) ? "&&" : "||";
		tmp = ft_4strjoin(str, token, ast, "");
		ft_strdel(&str);
		ft_strdel(&ast);
		str = tmp;
		cmd = cmd->next;
	}
	return (str);
}

/******************************* jobs begin ***********************************/

typedef struct			s_process
{
	char				**argv;				/* for exec (cmd arguments) */
	char				**env;				/* for exec (cmd env)*/
	pid_t				pid;				/* process ID */
	char				completed;			/* true if process has completed */
	char				stopped;			/* true if process has stopped */
	int					status;				/* reported status value */
	struct s_process	*next;				/* next process in pipeline */
}						t_process;

typedef struct			s_job
{
	char				*command;			/* command line, used for messages */
	t_process			*first_process;		/* list of processes in this job */
	pid_t				pgid;				/* process group ID */
	char				notified;			/* true if user told about stopped job */
	int					fd_in;				/* standard input */
	int					fd_out;				/* standard output */
	int					fd_err;				/* standard error */
	t_and_or			*cmd;				/* cmd origin */
	struct s_job		*next;				/* next active job */
}						t_job;

int		job_control(t_and_or *cmd, int bg)
{
	int	ret;

	ret = execute(cmd, bg);
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
	update_proc(pid,(WIFEXITED(status) || WIFSIGNALED(status) ? 0 : 2), status);
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
	update_proc(pid, status, 1);
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
	int		sig;

	(void)bg;
	sig = WIFEXITED(status) ? WEXITSTATUS(status) : 0;
	sig = WIFSIGNALED(status) ? WTERMSIG(status) : sig;
	sig = WIFSTOPPED(status) ? WSTOPSIG(status) : sig;
	p = g_var.proc;
	while (p)
	{
		if (p->ppid == pid)
			break ;
		p = p->next;
	}
	if (p)
		ft_print(STDOUT, "", p->index, p->c, ft_strsignal(sig), p->str);
	else if (WIFSIGNALED(status) && sig != 2)
		ft_print(STDOUT, "%s: %d", ft_strsignal(sig), sig);
	else
		ft_print(STDOUT, "error updating %d: p not found", pid);
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
		if (p->done == 0)
			ft_print(STDOUT,"running\t\t%s\n", p->str);
		else if (p->done == 1)
			ft_print(STDOUT,"done\t\t%s\n", p->str);
		else if (p->done == 2)
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
