/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 06:47:00 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/12 06:54:49 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strnew(size_t size)
{
	char	*p;
	size_t	i;

	i = 0;
	if (NULL == (p = (char*)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	while (i < size + 1)
	{
		*(p + i) = '\0';
		i++;
	}
	return (p);
}
