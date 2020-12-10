/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 23:26:27 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/09 23:18:06 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t		i;

	i = 0;
	while (i < n && n > 0)
	{
		*(unsigned char*)(dst + i) = *(unsigned char*)(src + i);
		if (*(unsigned char*)(src + i) == (unsigned char)c)
			return (void*)(dst + i + 1);
		i++;
	}
	return (NULL);
}
