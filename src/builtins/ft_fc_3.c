/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <sazouaka@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 16:47:47 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/04 16:47:50 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

int		get_str_index(char *s)
{
	int		len;
	t_hist	*node;

	len = ft_strlen(s);
	node = g_var.history;
	if (!node)
		return (0);
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

int		verify_index(int index)
{
	t_hist	*node;

	node = g_var.history;
	if (!node)
		return (0);
	while (node->next)
	{
		if (index == node->index)
			return (index);
		node = node->next;
	}
	return (node->index);
}

void	get_index_hist_first_1(int *index, t_hist *head)
{
	*index = get_last_hist();
	if (head)
		*index = (*index <= 16) ? head->index : *index - 15;
}

int		get_index_hist_first(char *s, int l)
{
	int		index;
	char	*tmp;

	tmp = fc_history_remove();
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
		get_index_hist_first_1(&index, g_var.history);
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
			index = get_last_hist() + ft_atoi(s);
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
