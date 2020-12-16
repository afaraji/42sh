/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 17:49:23 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/16 17:49:29 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/parse.h"
#include "../../inc/expansion.h"

void	push_to_list(t_l **head, char *s)
{
	t_l		*node;

	if (*head == NULL)
	{
		*head = fill_get_args(s);
		ft_strdel(&s);
		return ;
	}
	node = *head;
	while (node->next)
		node = node->next;
	node->next = fill_get_args(s);
	ft_strdel(&s);
}

char	**split_expanssion(char *s)
{
	t_l		*list;
	int		i;
	int		quoted;
	int		j;

	i = 0;
	j = 0;
	quoted = 0;
	list = NULL;
	while (s[i])
	{
		if (s[i] == QUOTE || s[i] == DQUOTE)
			quoted = (quoted == 0) ? 1 : 0;
		else if (quoted == 0 && (s[i] == ' ' || s[i] == '\n' || s[i] == '\t')
												&& (i == 0 || s[i - 1] != '\\'))
		{
			push_to_list(&list, ft_strsub(s, 0, i));
			s = s + i + 1;
			i = -1;
		}
		i++;
	}
	push_to_list(&list, ft_strsub(s, 0, i));
	return (list_to_tab(list));
}

void	var_sub_suit(t_l *head, char **t, t_l *next_node)
{
	t_l		*node;
	int		i;

	i = 1;
	node = head;
	while (t[i])
	{
		node->next = fill_get_args(t[i]);
		node = node->next;
		i++;
	}
	free_tab(t);
	node->next = next_node;
}

t_l		*var_sub(t_l *head)
{
	t_l		*next_node;
	char	**t;

	next_node = head->next;
	if (expansions_dispatcher(&(head->data)))
	{
		g_var.errno = 11;
		return (head);
	}
	t = split_expanssion(head->data);
	ft_strdel(&(head->data));
	if (!t || !t[0])
	{
		head->data = ft_strdup("");
		head->next = next_node;
		return (head);
	}
	head->data = ft_strdup(t[0]);
	var_sub_suit(head, t, next_node);
	return (head);
}

int		param_expand(t_l *list)
{
	t_l		*node;

	node = list;
	while (node)
	{
		node = var_sub(node);
		if (g_var.errno)
		{
			free_l(list);
			return (1);
		}
		node = node->next;
	}
	return (0);
}
