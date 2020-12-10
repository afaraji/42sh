/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 08:39:09 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/12 09:57:07 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	size_t	len;
	size_t	i;
	char	*p;

	i = 0;
	p = NULL;
	if (s == NULL)
		return (NULL);
	len = ft_strlen(s);
	if ((p = (char*)malloc(sizeof(char) * (len + 1))) == NULL)
		return (NULL);
	while (i < (len) && *(s + i) != '\0')
	{
		*(p + i) = f(*(char*)(s + i));
		i++;
	}
	*(p + i) = '\0';
	return (p);
}
