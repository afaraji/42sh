/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 08:15:54 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/09 14:55:32 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char *t;

	if ((t = (char*)malloc(sizeof(char) * (ft_strlen(s1) + 1))) == NULL)
		return (NULL);
	ft_memcpy(t, s1, ft_strlen(s1));
	*(t + ft_strlen(s1)) = '\0';
	return (t);
}
