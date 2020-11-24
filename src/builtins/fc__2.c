/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <sazouaka@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 18:18:50 by sazouaka          #+#    #+#             */
/*   Updated: 2020/11/20 18:18:52 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

#define S_OPT	0	//s
#define L_OPT	1	//l
#define N_OPT	2	//n
#define R_OPT	3	//r
#define E_OPT	4	//e

int		get_opt_str(int opt[5], char *str)
{
	int		i;
	int		e;

	i = 0;
	e = 0;
	while (str[i])
	{
		if (str[i] == 's')
			opt[S_OPT] = 1;
		else if (str[i] == 'l')
			opt[L_OPT] = 1;
		else if (str[i] == 'n')
			opt[N_OPT] = 1;
		else if (str[i] == 'r')
			opt[R_OPT] = 1;
		else if (str[i] == 'e')
		{
			opt[E_OPT] = 1;
			e = 1;
		}
		else
		{
			ft_print(stderr, "fc: -%c: invalid option", str[i]);
			return (0);
		}
		i++;
	}
	if (e)
		return(2);
	return (1);
}

int		get_opt_av(int opt[5], char **av, char **editor)
{
	int		i;
	int		ret;

	i = 1;
	while (av[i] && av[i][0] == '-')
	{
		if (av[i][1] = '\0')
		{
			ft_print(stderr, "shell: fc: history specification out of range");
			return (-1);
		}
		if ((ret = get_opt_str(opt, &av[i][1])) == 0)
			return (-1);
		if (ret = 2)
		{
			if (av[i + 1])
			{
				i++;
				*editor = ft_strdup(av[i]);
			}
			else
			{
				ft_print(stderr, "shell: fc: -e: option requires an argument\n");
				return (-1);
			}
		}
		i++;
	}
	return (i);
}

char	*get_first(char **av)
{
	if (av && av[0])
	{
		return
	}
}

int		ft_fc_2(char *f, char *l, int opt[5], char *e)
{
	return (0);
}

int		ft_fc(char **av)
{
	int		i;
	int		opt[5] = {0, 0, 0, 0, 0};	// s = 0, l = 1, n = 2, r = 3, e = 4;
	char	*editor;
	char	*first;
	char	*last;

	i = get_opt_av(opt, av, &editor);
	if (i == -1)
		return (i);
	if (av[i] && av[i + 1] && av[i + 2])
	{
		ft_print(stderr, "shell: fc: too many args\n");
		return (-2);
	}
	first = (av[i]) ? av[i] : NULL;
	last = (av[i] && av[i + 1]) ? av[i + 1] : first;
	i = ft_fc_2(first, last, opt, editor);
	return (i);
}
