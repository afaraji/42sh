/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 21:33:36 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/11 23:25:42 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (*(unsigned char*)(s1) != *(unsigned char*)(s2))
		return (*(unsigned char*)(s1) - *(unsigned char*)(s2));
	while (*(unsigned char*)(s1 + i) && *(unsigned char*)(s2 + i))
	{
		if (*(unsigned char*)(s1 + i) != *(unsigned char*)(s2 + i))
			return (*(unsigned char*)(s1 + i) - *(unsigned char*)(s2 + i));
		i++;
	}
	if (*(unsigned char*)(s1 + i) == '\0' || *(unsigned char*)(s2 + i) == '\0')
		if (*(unsigned char*)(s1 + i) != *(unsigned char*)(s2 + i))
			return (*(unsigned char*)(s1 + i) - *(unsigned char*)(s2 + i));
	return (0);
}
