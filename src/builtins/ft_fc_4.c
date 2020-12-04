/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <sazouaka@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 16:49:10 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/04 16:49:11 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

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

int		get_last_hist(void)
{
	t_hist	*node;

	node = g_var.history;
	if (!node)
		return (0);
	while (node->next)
		node = node->next;
	return (node->index);
}

/*
**may ause leaks
*/

char	*fc_history_remove(void)
{
	t_hist	*node;
	t_hist	*prec;
	char	*str;

	node = g_var.history;
	if (!node)
		return (NULL);
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
	if (node == g_var.history)
		g_var.history = NULL;
	node = NULL;
	return (str);
}

void	fc_history_add(char *s, int l)
{
	t_hist	*node;

	if (l == 0)
		return ;
	if (g_var.history == NULL)
	{
		g_var.history = get_his_node(s, NULL, 1);
		return ;
	}
	node = g_var.history;
	while (node->next)
		node = node->next;
	if (node)
		node->next = get_his_node(s, node, node->index + 1);
	else
		node = get_his_node(s, NULL, 1);
}
