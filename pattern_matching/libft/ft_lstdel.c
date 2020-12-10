/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 22:04:54 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/17 22:23:05 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list	*local_next;

	while (*alst != NULL)
	{
		local_next = (*alst)->next;
		del((*alst)->content, (*alst)->content_size);
		free((*alst));
		*alst = local_next;
	}
}
