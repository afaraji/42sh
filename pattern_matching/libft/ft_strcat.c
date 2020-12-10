/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 15:59:15 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/07 16:40:05 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strcat(char *s1, const char *s2)
{
	int	i;
	int j;

	j = 0;
	i = 0;
	while (*(unsigned char*)(s1 + i) != '\0')
		i++;
	while (*(unsigned char*)(s2 + j) != '\0')
	{
		*(unsigned char*)(s1 + i) = *(unsigned char*)(s2 + j);
		i++;
		j++;
	}
	*(unsigned char*)(s1 + i) = '\0';
	return (s1);
}
