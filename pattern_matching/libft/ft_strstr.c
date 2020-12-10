/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 15:54:19 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/12 05:30:52 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	int		i;
	int		j;
	char	*k;

	k = NULL;
	i = 0;
	if (*needle == '\0')
		return (char*)(haystack);
	while (*(haystack + i) != '\0')
	{
		j = 0;
		while (*(haystack + i) == *(needle + j) && *(needle + j) != '\0')
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
