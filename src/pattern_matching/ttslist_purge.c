/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttslist_purge.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 09:53:51 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 13:10:36 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ttslist.h"

void	ttslist_purge(t_list_head *list, void (*purge)(void *))
{
	void	*content;

	while (list->first && (content = ttslist_splice(list, list->first)))
		purge(content);
}

int		ttslist_remove_node_with_content(t_list_head *list, void *content)
{
	t_list_node	*node;

	if (!list)
		return (0);
	node = list->first;
	while (node)
	{
		if (node->content == content)
		{
			ttslist_splice(list, node);
			return (1);
		}
		node = node->next;
	}
	return (0);
}
