/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsignal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 18:09:13 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:40:36 by awali-al         ###   ########.fr       */
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
	if (sig == 16 || sig == 19 || sig == 20 || sig == 23 || sig == 28 ||
																	sig == 29)
		return ("undefined");
	if (sig <= 12)
		return (ft_getsigstr1_12(sig));
	return (ft_getsigstr13_31(sig));
}
