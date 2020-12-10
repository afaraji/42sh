/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 20:43:42 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/11 21:33:07 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	int		i;
	int		j;
	char	*k;

	k = NULL;
	i = 0;
	if (*needle == '\0')
		return (char*)(haystack);
	while (*(haystack + i) != '\0' && (size_t)i < len)
	{
		j = 0;
		while (*(haystack + i) == *(needle + j) && *(needle + j) != '\0'
				&& (size_t)i < len)
		{
			j++;
			i++;
		}
		if (*(needle + j) == '\0')
		{
			k = (char*)(haystack + i - j);
			return (k);
		}
		i = i - j + 1;
	}
	return (k);
}
