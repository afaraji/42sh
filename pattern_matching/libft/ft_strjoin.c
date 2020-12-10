/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 21:01:03 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/13 01:08:55 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;
	char	*s;

	j = -1;
	i = -1;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	if (NULL == (s = (char*)malloc(sizeof(char) * (len1 + len2 + 1))))
		return (NULL);
	while (++i < len1)
		*(s + i) = *(s1 + i);
	while (++j < len2)
	{
		*(s + i) = *(s2 + j);
		i++;
	}
	*(s + i) = '\0';
	return (s);
}
