/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 12:28:44 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/12 21:00:28 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*p;
	size_t	j;

	if (s == NULL)
		return (NULL);
	j = 0;
	i = (size_t)start;
	if ((p = (char*)malloc(sizeof(char) * (len + 1))) == NULL)
		return (NULL);
	while (j < len && *(s + i) != '\0')
	{
		*(p + j) = *(s + i);
		i++;
		j++;
	}
	*(p + j) = '\0';
	return (p);
}
