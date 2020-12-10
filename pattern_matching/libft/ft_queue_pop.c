/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_queue_pop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/21 04:23:21 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/21 04:30:26 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_queue_pop(t_list *alst)
{
	void	*elem;

	while (alst->next->next)
		alst = alst->next;
	elem = alst->next->content;
	free(alst->next);
	free(alst->next->next);
	alst->next = NULL;
	return (elem);
}
