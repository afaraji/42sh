/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 17:26:42 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/17 20:35:51 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	int		j;
	int		t;

	i = 0;
	j = ft_strlen(dst) + ft_strlen(src);
	t = ft_strlen(dst);
	while (*dst != '\0' && i < size)
	{
		i++;
		dst++;
	}
	if (i == size)
		return (size + ft_strlen(src));
	i = 0;
	while (*src != '\0' && (i + t) < size - 1)
	{
		*dst = *src;
		src++;
		dst++;
		i++;
	}
	*(dst) = '\0';
	return (j);
}
