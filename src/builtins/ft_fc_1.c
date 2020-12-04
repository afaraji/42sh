/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <sazouaka@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 16:40:31 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/04 16:40:33 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

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

void	get_fc_list_2(t_hist *start, t_hist *end, t_hist **list, t_hist **node)
{
	if (start && start == end)
	{
		if (!(*list))
			*list = get_his_node(start->hist_str, NULL, start->index);
		else
			(*node)->next = get_his_node(start->hist_str, *node, start->index);
	}
}

t_hist	*get_fc_list_1(t_hist *start, t_hist *end, int reverse)
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
	get_fc_list_2(start, end, &list, &node);
	return (list);
}

t_hist	*get_fc_list(char *first_s, char *last_s, int l)
{
	int		first;
	int		last;
	t_hist	*start;
	t_hist	*end;

	first = 0;
	last = 0;
	first = get_index_hist_first(first_s, l);
	last = get_index_hist_last(last_s, l, first);
	if (!first || !last)
		return (NULL);
	start = get_hist_node(first);
	end = get_hist_node(last);
	if (first < last)
		return (get_fc_list_1(start, end, 0));
	return (get_fc_list_1(start, end, 1));
}
