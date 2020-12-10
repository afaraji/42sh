/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/12 08:01:58 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/19 20:34:16 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	len;
	unsigned int	i;

	i = 0;
	if (s != NULL && f != NULL)
	{
		len = (int)ft_strlen(s);
		while (i < len && *(s + i) != '\0')
		{
			f(i, s + i);
			i++;
		}
	}
	else
		return ;
}
