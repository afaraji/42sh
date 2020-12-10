/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 05:14:20 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/20 01:49:28 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	void			*p;

	if (n == 0)
		return (NULL);
	p = (void*)s;
	i = 0;
	while (i < n)
	{
		if (*(unsigned char*)(s + i) == (unsigned char)c)
			return (p + i);
		i++;
	}
	if (i < n)
		return (p + i);
	return (NULL);
}
