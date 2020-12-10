/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stack_pop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 01:41:55 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/21 05:06:57 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_stack_pop(t_list **alst)
{
	t_list *elem;

	elem = *alst;
	*alst = (*alst)->next;
	elem->next = NULL;
	return (elem->content);
}
