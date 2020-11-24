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
			ft_print(stderr, "fc: -%c: invalid option", str[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

int		get_opt_av(int opt[5], char **av, char **editor)
{
	int		i;

	i = 1;
	while (av[i] && av[i][0] == '-')
	{
		if (av[i][1] = '\0')
		{
			ft_print(stderr, "shell: fc: history specification out of range");
			return (-1);
		}
		if (is_all_digits(&av[i][1]))
		{
			i++;
			break ;
		}
		if (get_opt_str(opt, &av[i][1]) == 0)
			return (-1);
		if (opt[E_OPT] == 1)
		{
			if (av[i + 1])
			{
				i++;
				*editor = ft_strdup(av[i]);
				opt[E_OPT] = 2;
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

t_hist	*reverse_list(t_hist *list)//not revers !!!!!
{
	t_hist	*node;

	node = list;
	if (reverse == 0)
	{
		while (node)
		{
			printf("%s", node->hist_str);
			node = node->next;
		}
	}
	else
	{
		while (node->next)
			node = node->next;
		while (node)
		{
			printf("%s", node->hist_str);
			node = node->prec;
		}
	}
}

int		verify_index(int index)
{
	t_hist	*node;

	node = g_var.history;
	while (node)
	{
		if (index == node->index)
			return (index);
		node = node->next;
	}
	return (0);
}

int		get_str_index(char *s)
{
	int		len;
	t_hist	*node;

	len = ft_strlen(s);
	node = g_var.history;
	while (node->next)
		node = node->next;
	while (node)
	{
		if (!ft_strncmp(s, node->hist_str, len))
			return (node->index);
		node = node->prec;
	}
	return (0);
}

int		get_last_hist(void)
{
	t_hist	*node;

	node = g_var.history;
	while (node->next)
		node = node->next;
	return (node->index);
}

int		get_index_hist_first(char *s, int l)
{
	int		index;

	if (s)
	{
		if (is_all_digits(s))
			index = verify_index(ft_atoi(s));
		else if (s[0] == '-' && is_all_digits(&s[1]))
		{
			index = get_last_hist() + ft_atoi(s) + 1;
			index = (index > 0) ? index : g_var.history->index;
		}
		else
			index = get_str_index(s);
	}
	else if (l)
	{
		index = get_last_hist();
		index = (index <= 16) ? g_var.history->index : index - 15;
	}
	else
		index = get_last_hist();
	return (index);
}

int		get_index_hist_last(char *s, int l, int first_index)
{
	int		index;

	if (s)
	{
		if (is_all_digits(s))
			index = verify_index(ft_atoi(s));
		else if (s[0] == '-' && is_all_digits(&s[1]))// ??!! was verifying this
		{
			index = get_last_hist() + ft_atoi(s) + 1;
			index = (index > 0) ? index : g_var.history->index;
		}
		else
			index = get_str_index(s);
	}
	else if (l)
		index = get_last_hist();
	else
		index = first_index;
	return (index);
}

t_hist	*get_fc_list(char *first, char *last, int r, int l)
{
	t_hist	*list;
	int		first_index;
	int		last_index;

	first_index = get_index_hist_first(first, l);
	last_index = get_index_hist_last(last, l, first_index);
	if (!first_index || !last_index)
		return (NULL);
	return (list);
}

int		fc_print_list(t_hist *list, int n)
{
	if (n)
	{
		while (list)
		{
			ft_print(STDOUT, "\t\t%s\n", list->hist_str);
			list = list->next;
		}
	}
	else
	{
		while (list)
		{
			ft_print(STDOUT, "%d\t%s\n", list->index, list->hist_str);
			list = list->next;
		}
	}
	return (0);
}

int		fc_exec(t_hist *list, char *editor)
{
	//
}

int		ft_fc_2(char *f, char *l, int opt[5], char *e)
{
	t_hist	*list;

	list = get_fc_list(f, l, opt[R_OPT], opt[L_OPT]);
	if (list == NULL)
	{
		ft_print(STDOUT, "fc: no events in that range\n");
		return (1);
	}
	if (opt[L_OPT])
		return (fc_print_list(list, opt[N_OPT]));
	else
		return (fc_exec(list, e));
}

int		ft_fc(char **av)
{
	int		i;
	int		opt[5] = {0, 0, 0, 0, 0};	// s = 0, l = 1, n = 2, r = 3, e = 4;
	char	*editor;
	char	*first;
	char	*last;

	if (g_var.history == NULL)
		return (-1);
	i = get_opt_av(opt, av, &editor);
	if (i == -1)
		return (i);
	if (opt[S_OPT])
	{
		return (fc_do_s());
	}
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
