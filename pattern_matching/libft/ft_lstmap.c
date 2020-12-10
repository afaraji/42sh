/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 23:22:40 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/18 22:55:07 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*list;

	if (!lst)
		return (NULL);
	list = ft_lstnew(lst->content, lst->content_size);
	list = f(list);
	if (lst->next)
		lst = lst->next;
	while (lst)
	{
		ft_lstaddlast(list, f(lst));
		lst = lst->next;
	}
	return (list);
}
