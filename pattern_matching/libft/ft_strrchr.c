/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 18:16:01 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/11 15:49:32 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strrchr(const char *s, int c)
{
	char	*f;
	char	*ss;

	f = NULL;
	ss = (char*)s;
	while (*ss != '\0')
	{
		if (*ss == (char)c)
			f = ss;
		ss++;
	}
	if ((char)c == '\0')
		return (ss);
	return (f);
}
