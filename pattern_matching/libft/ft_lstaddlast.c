/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstaddlast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 23:36:29 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/18 22:55:03 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	ft_lstaddlast(t_list *alst, t_list *new)
{
	while (alst->next != NULL)
		alst = alst->next;
	new->next = NULL;
	alst->next = new;
}
