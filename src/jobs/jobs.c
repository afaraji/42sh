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
		str[0] = "broken pipe";
		str[1] = "Alarm clock";
		str[2] = "terminated";
		str[4] = "stoped";
		str[5] = "stoped";
		str[8] = "stoped";
		str[9] = "stoped";
		str[11] = "Cputime limit exceeded";
		str[12] = "Filesize limit exceeded";
		str[13] = "Virtual timer expired";
		str[14] = "Profiling timer expired";
		str[17] = "User defined signal 1";
		str[18] = "User defined signal 2";
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
	ft_print(STDERR, "killed by signal: %d\n", sig);
	return (sig);
}

int		stopped_by(int sig)
{
	ft_print(STDERR, "stopped by signal: %d\n", sig);
	return (sig);
}

int		job_control(t_and_or *cmd, int bg)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, WNOHANG | WUNTRACED);
	if (pid > 0)
	{
		printf("pid[%d] has changed state to [%x]\n", pid, status);
	}
	if ((pid = fork()) < 0)
		return (-1);//print error and exit instad ?!!
	if (pid == 0)
	{
		pid = setsid();
		if (!bg)
			tcsetpgrp (STDIN, pid); // dont forget return value !!!
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
		// exec cmd;
	}
	else
	{
		setpgid(pid, pid);
		if (!bg)
		{
			if (waitpid(pid, &status, WUNTRACED | WCONTINUED) < 0)
				return (-2);
			// The shell determines that a stop was sent to the child by looking at the status value
			if (WIFEXITED(status))
				return (WEXITSTATUS(status));
			if (WIFSIGNALED(status))
				return (killed_by(WTERMSIG(status)));
			if (WIFSTOPPED(status))
				return (stopped_by(WSTOPSIG(status)));
			signal (SIGTTOU, SIG_IGN);
			signal (SIGTTIN, SIG_IGN);
			tcsetpgrp (STDIN, getpid()); // dont forget return value !!!
			// add child as suspended process in process list
		}

	}
	return (0);
}
