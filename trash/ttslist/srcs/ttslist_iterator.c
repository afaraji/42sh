/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttslist_iterator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 09:53:26 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 14:18:31 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ttslist.h"

void		*ttslist_iter_content(t_list_head *head)
{
	void				*result;

	if (!head->iterator)
		return (NULL);
	result = head->iterator->content;
	head->iterator = head->iterator->next;
	return (result);
}

t_list_node	*ttslist_iter(t_list_head *head)
{
	t_list_node			*result;

	if (!head->iterator)
		return (NULL);
	result = head->iterator;
	head->iterator = head->iterator->next;
	return (result);
}

void		*ttslist_get_id_content(t_list_head *head, int id)
{
	t_list_node	*iterator;
	void		*content;

	iterator = head->first;
	while (iterator)
	{
		content = iterator->content;
		if (!id)
			return (content);
		id--;
		iterator = iterator->next;
	}
	return (NULL);
}

int			ttslist_get_content_id(t_list_head *head, void *content)
{
	void		*element;
	t_list_node	*iterator;
	int			id;

	id = 0;
	iterator = head->first;
	while (iterator)
	{
		element = iterator->content;
		if (content == element)
			return (id);
		id++;
		iterator = iterator->next;
	}
	return (-1);
}
