/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 16:40:16 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/09 14:50:00 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	int		i;
	size_t	j;

	i = 0;
	j = 0;
	while (*(unsigned char*)(s1 + i) != '\0')
		i++;
	while (*(unsigned char*)(s2 + j) != '\0' && j < n)
	{
		*(unsigned char*)(s1 + i) = *(unsigned char*)(s2 + j);
		i++;
		j++;
	}
	*(unsigned char*)(s1 + i) = '\0';
	return (s1);
}
