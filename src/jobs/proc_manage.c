/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 17:49:31 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/15 20:02:14 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/jobs.h"

void	bg_jobs(void)
{
	int		pid;
	int		status;

	while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
	{
		find_job_and_update(pid, status);
	}
	notify_user();
}
