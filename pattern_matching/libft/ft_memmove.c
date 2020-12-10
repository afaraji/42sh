/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 04:00:20 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/11 15:46:00 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	tmp[len];
	void	*p;

	p = dst;
	if (len > 65535)
		return (NULL);
	if (p == src)
		return (p);
	else if (p > src && p < src + len)
	{
		ft_memcpy(tmp, src, len);
		ft_memcpy(p, tmp, len);
	}
	else
		ft_memcpy(p, src, len);
	return (p);
}
