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

int		get_opt_str(int opt[5], char *str)
{
	int		i;

	i = 0;
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
			opt[E_OPT] = 1;
		else
		{
			ft_print(STDERR, "fc: -%c: invalid option", str[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

int		get_opt_av_1(int opt[5], char **av, char **editor, int *i)
{
	if (av[*i + 1])
	{
		(*i)++;
		*editor = ft_strdup(av[*i]);
		opt[E_OPT] = 2;
		return (0);
	}
	else
	{
		ft_print(STDERR, "shell: fc: -e: option requires an argument\n");
		return (-1);
	}
}

int		get_opt_av(int opt[5], char **av, char **editor)
{
	int		i;

	i = 0;
	while (i < 5)
		opt[i++] = 0;
	i = 1;
	while (av[i] && av[i][0] == '-')
	{
		if (av[i][1] == '-')
			return (i + 1);
		if (av[i][1] == '\0')
		{
			ft_print(STDERR, "shell: fc: history specification out of range\n");
			return (-1);
		}
		if (is_all_digits(&av[i][1]))
			return (i);
		if (get_opt_str(opt, &av[i][1]) == 0)
			return (-1);
		if (opt[E_OPT] == 1)
			if (get_opt_av_1(opt, av, editor, &i) == -1)
				return (-1);
		i++;
	}
	return (i);
}

int		ft_fc_1(char *f, char *l, int opt[5], char *e)
{
	t_hist	*list;

	list = get_fc_list(f, l, opt[L_OPT]);
	if (list == NULL)
	{
		ft_print(STDOUT, "fc: no events in that range\n");
		return (1);
	}
	if (opt[L_OPT])
		return (fc_print_list(list, opt[N_OPT], opt[R_OPT]));
	else
		return (fc_add_to_file(list, e, opt[R_OPT]));
}

int		ft_fc(char **av)
{
	int		i;
	int		opt[5];
	char	*editor;
	char	*first;
	char	*last;

	editor = NULL;
	i = get_opt_av(opt, av, &editor);
	if (i == -1)
		return (i);
	if (opt[S_OPT])
		return (fc_do_s(av, i));
	if (av[i] && av[i + 1] && av[i + 2])
	{
		ft_print(STDERR, "shell: fc: too many args\n");
		return (-2);
	}
	first = (av[i]) ? av[i] : NULL;
	last = (opt[L_OPT]) ? NULL : first;
	last = (av[i] && av[i + 1]) ? av[i + 1] : last;
	i = ft_fc_1(first, last, opt, editor);
	return (i);
}
