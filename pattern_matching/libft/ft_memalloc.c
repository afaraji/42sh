/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 05:54:16 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/12 06:08:03 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void	*t;
	size_t	i;

	i = 0;
	if (size == 0 || size > 65535)
		return (NULL);
	if (!(t = malloc(size)))
		return (NULL);
	ft_bzero(t, size);
	return (t);
}
