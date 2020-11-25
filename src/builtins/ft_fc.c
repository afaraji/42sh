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
			ft_print(STDERR, "fc: -%c: invalid option", str[i]);
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
		if (av[i][1] == '\0')
		{
			ft_print(STDERR, "shell: fc: history specification out of range");
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
				ft_print(STDERR, "shell: fc: -e: option requires an argument\n");
				return (-1);
			}
		}
		i++;
	}
	return (i);
}

int		verify_index(int index)
{
	t_hist	*node;

	node = g_var.history;
	while (node->next)
	{
		if (index == node->index)
			return (index);
		node = node->next;
	}
	return (node->index);
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

char	*fc_history_remove(void)
{
	t_hist	*node;
	t_hist	*prec;
	char	*str;

	node = g_var.history;
	while (node->next)
		node = node->next;
	prec = node->prec;
	if (prec)
		prec->next = NULL;
	node->prec = NULL;
	node->next = NULL;
	ft_strdel(&(node->s_chr));
	str = ft_strdup(node->hist_str);
	ft_strdel(&(node->hist_str));
	free(node);
	return (str);
}

void	fc_history_add(char *s, int l)
{
	t_hist	*node;

	if (l == 0)
		return ;
	node = g_var.history;
	while (node->next)
		node = node->next;
	if (node)
		node->next = get_his_node(s, node, node->index + 1);
	else
		node = get_his_node(s, NULL, 1);
}

int		get_index_hist_first(char *s, int l)
{
	int		index;
	char	*tmp;

	tmp = fc_history_remove();// save
	if (s)
	{
		if (is_all_digits(s))
			index = verify_index(ft_atoi(s));
		else if (s[0] == '-' && is_all_digits(&s[1]))
		{
			index = get_last_hist() + ft_atoi(s) + 1;// + 1 !!! may need to remove 1
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
	fc_history_add(tmp, l);
	return (index);
}

int		get_index_hist_last(char *s, int l, int first_index)
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
		index = get_last_hist() - 1;
	else
		index = first_index;
	return (index);
}

t_hist	*get_fc_list_2(t_hist *start, t_hist *end, int reverse)
{
	t_hist	*list;
	t_hist	*node;

	list = NULL;
	while (start && start != end)
	{
		if (!list)
		{
			list = get_his_node(start->hist_str, NULL, start->index);
			node = list;
		}
		else
		{
			node->next = get_his_node(start->hist_str, node, start->index);
			node = node->next;
		}
		start = (reverse == 0) ? start->next : start->prec;
	}
	if (start && start == end)
	{
		if (!list)
			list = get_his_node(start->hist_str, NULL, start->index);
		else
			node->next = get_his_node(start->hist_str, node, start->index);
	}
	return (list);
}

t_hist	*get_hist_node(int index)
{
	t_hist	*node;

	node = g_var.history;
	while (node)
	{
		if (node->index == index)
			return (node);
		node = node->next;
	}
	return (NULL);
}

t_hist	*get_fc_list(char *first_s, char *last_s, int l)
{
	int		first;
	int		last;
	// int		reverse;
	t_hist	*start;
	t_hist	*end;

	first = 0;
	last = 0;
	first = get_index_hist_first(first_s, l);
	last = get_index_hist_last(last_s, l, first);
	if (!first || !last)
		return (NULL);
	// reverse = ((first < last && r == 0) || (first > last && r == 1)) ? 0 : 1;
	// ft_print(STDERR, "fisrts[%d] - last[%d]\n", first, last);
	start = get_hist_node(first);
	end = get_hist_node(last);
	// ft_print(STDERR, "===> [%d:%s]-[%d:%s]\n",start->index, start->hist_str,end->index, end->hist_str);
	if (first < last)
		return (get_fc_list_2(start, end, 0));
	return (get_fc_list_2(start, end, 1));
}

/*
**	if (first < last) ok		|	if (r == 0) ok
**	if (first > last) reverse	|	if (r == 1) reverse
*/

int		fc_print_list(t_hist *list, int n, int r)
{
	t_hist	*node;

	node = list;
	while (r && node->next)
		node = node->next;
	if (n)
	{
		while (node)
		{
			ft_print(STDOUT, "\t%s\n", node->hist_str);
			node = (r) ? node->prec : node->next;
		}
	}
	else
	{
		while (node)
		{
			ft_print(STDOUT, "%d\t%s\n", node->index, node->hist_str);
			node = (r) ? node->prec : node->next;
		}
	}
	return (0);
}

int		fc_exec(t_hist *list, char *editor, int r)
{
	t_hist	*node;
//	stoped here need to test this shit and if it prints good
//	adress -S or continue to output to the file and exec then go back -s ?
	node = list;
	while (r && node->next)
		node = node->next;
	ft_print(STDOUT, "===== %s ====\n", editor);
	while (node)
	{
		ft_print(STDOUT, "%s\n", node->hist_str);
		node = (r) ? node->prec : node->next;
	}
	return (0);
}

int		ft_fc_2(char *f, char *l, int opt[5], char *e)
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
		return (fc_exec(list, e, opt[R_OPT]));
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
		// return (fc_do_s());
		return (0);
	}
	if (av[i] && av[i + 1] && av[i + 2])
	{
		ft_print(STDERR, "shell: fc: too many args\n");
		return (-2);
	}
	first = (av[i]) ? av[i] : NULL;
	last = (av[i] && av[i + 1]) ? av[i + 1] : first;
	i = ft_fc_2(first, last, opt, editor);
	return (i);
}
