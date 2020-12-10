/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 23:23:10 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/11 23:34:38 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int i;

	i = 0;
	if (n == 0)
		return (0);
	if (*(unsigned char*)(s1) != *(unsigned char*)(s2))
		return (*(unsigned char*)(s1) - *(unsigned char*)(s2));
	while ((*(unsigned char*)(s1 + i) && *(unsigned char*)(s2 + i))
			&& i < (int)n)
	{
		if (*(unsigned char*)(s1 + i) != *(unsigned char*)(s2 + i))
			return (*(unsigned char*)(s1 + i) - *(unsigned char*)(s2 + i));
		i++;
	}
	if ((*(unsigned char*)(s1 + i) == '\0' || *(unsigned char*)(s2 + i) == '\0')
			&& i < (int)n)
		if (*(unsigned char*)(s1 + i) != *(unsigned char*)(s2 + i))
			return (*(unsigned char*)(s1 + i) - *(unsigned char*)(s2 + i));
	return (0);
}
