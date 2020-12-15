/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 19:09:00 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 19:25:15 by awali-al         ###   ########.fr       */
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

extern pid_t	g_current_job;
extern pid_t	g_previous_job;

int		get_opt(char **av, int *index)
{
	int		i;
	int		j;
	int		opt;

	i = 1;
	opt = 0;
	while (av[i] && (av[i][0] == '-' && av[i][1] && av[i][1] != '-'))
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

t_job	*get_pid_str(char *s)
{
	t_job	*j;
	int		len;

	len = ft_strlen(s);
	j = g_var.job;
	if (*s == '?')
	{
		s++;
		while (j)
		{
			if (ft_strstr(j->command, s))
				return (j);
			j = j->next;
		}
	}
	while (j)
	{
		if (!ft_strncmp(j->command, s, len))
			return (j);
		j = j->next;
	}
	ft_print(STDERR, "shell: job_util: %s: no such job.\n", s);
	return (NULL);
}

t_job	*get_pid_n_plus_min(char c, char *s)
{
	t_job	*j;
	int		index;
	pid_t	pgid;

	j = g_var.job;
	index = (s != NULL) ? ft_atoi(s) : -1;
	pgid = (c == '+') ? g_current_job : 0;
	pgid = (c == '-') ? g_previous_job : pgid;
	while (j)
	{
		if (index == j->index || (pgid != 0 && pgid == j->pgid))
			return (j);
		j = j->next;
	}
	ft_putstr_fd("shell: job_util: ", STDERR);
	if (c == '+')
		ft_putstr_fd("current", STDERR);
	else if (c == '-')
		ft_putstr_fd("previous", STDERR);
	else
		ft_putstr_fd(s, STDERR);
	ft_putstr_fd(": no such job.\n", STDERR);
	return (NULL);
}
