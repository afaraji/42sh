/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 21:55:13 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/13 00:28:49 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strtrim(char const *s)
{
	size_t	i;
	size_t	len;
	char	*c;

	if (s == NULL)
		return (NULL);
	while ((*s == ' ' || *s == '\n' || *s == '\t') && *s != '\0')
		s++;
	len = ft_strlen(s);
	i = len;
	while (--i > 0 && *s &&
			(*(s + i) == ' ' || *(s + i) == '\n' || *(s + i) == '\t'))
		if (*(s + i) == ' ' || *(s + i) == '\n' || *(s + i) == '\t')
			len--;
	if ((c = (char*)malloc(sizeof(char) * (len + 1))) == NULL)
		return (NULL);
	i = -1;
	s--;
	while (*++s && ++i < len)
		*(c + i) = *s;
	*(c + len) = '\0';
	return (c);
}
