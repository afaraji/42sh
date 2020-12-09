/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 22:00:18 by sazouaka          #+#    #+#             */
/*   Updated: 2020/10/29 23:56:26 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

void	ft_signal(void)//not used yet
{
	g_var.shell_is_interactive = isatty (STDIN);
	if (g_var.shell_is_interactive)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGTTIN, SIG_IGN);
		signal(SIGTTOU, SIG_IGN);
		signal(SIGCHLD, SIG_DFL);
		if (setpgid (g_var.shell_pid, g_var.shell_pid) < 0)
		{
			perror ("Couldn't put the shell in its own process group: ");
			exit (1);
		}
		system_calls("Tcsetpgrp: ", tcsetpgrp (STDIN, g_var.shell_pid), -1);
		// if (ft_set_attr(0))
		// {
		// 	perror ("Couldn't set attributes: ");
		// 	exit (1);
		// }
	}
}
