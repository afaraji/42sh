/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 17:49:31 by afaraji           #+#    #+#             */
/*   Updated: 2020/11/03 17:49:48 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"


int		find_job_and_update(pid_t pid, int status);
void	notify_user(void);



void	bg_jobs(void)
{
	// t_proc	*proc;
	int		pid;
	int		status;

	while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
	{
		// printf("debug_msg:updating proc [%d]-status[%d]\n", pid, status);
		find_job_and_update(pid, status);
	}
	notify_user();
}
