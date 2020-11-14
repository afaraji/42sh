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
