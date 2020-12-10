/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 17:58:13 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/19 20:48:33 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		lenght(int n)
{
	int		i;
	long	a;

	i = 0;
	if (n > 0)
	{
		a = 1;
		while ((long)n > a)
		{
			a *= 10;
			i++;
		}
	}
	if (n < 0)
	{
		a = -1;
		while ((long)n < a)
		{
			a *= 10;
			i++;
		}
	}
	return (i);
}

static char		*ft_itp(int n)
{
	int		l;
	char	*c;
	int		q;

	l = lenght(n);
	if (!(c = (char*)malloc(sizeof(char) * (l + 1))))
		return (NULL);
	*(c + l) = '\0';
	while (--l >= 0)
	{
		q = n % 10;
		n = n / 10;
		*(c + l) = q + 48;
	}
	return (c);
}

static char		*ft_itn(int n)
{
	char	*c;
	int		l;
	int		q;

	l = lenght(n);
	if (!(c = (char*)malloc(sizeof(char) * (l + 2))))
		return (NULL);
	*c++ = '-';
	*(c + l) = '\0';
	while (--l >= 0)
	{
		q = n % 10;
		n = n / 10;
		*(c + l) = -q + 48;
	}
	return (--c);
}

static char		*ft_iten(int n)
{
	char	*c;

	if (n == -10)
	{
		if (!(c = (char*)malloc(sizeof(char) * 4)))
			return (NULL);
		c[0] = '-';
		c[1] = '1';
		c[2] = '0';
		c[3] = '\0';
	}
	else
	{
		if (!(c = (char*)malloc(sizeof(char) * 3)))
			return (NULL);
		c[0] = '1';
		c[1] = '0';
		c[2] = '\0';
	}
	return (c);
}

char			*ft_itoa(int n)
{
	char	*c;

	if (n > -10 && n < 0)
	{
		if (!(c = (char*)malloc(sizeof(char) * 3)))
			return (NULL);
		c[0] = '-';
		c[1] = -n + 48;
		c[2] = '\0';
	}
	if (n >= 0 && n < 10)
	{
		if (!(c = (char*)malloc(sizeof(char) * 2)))
			return (NULL);
		c[0] = n + 48;
		c[1] = '\0';
	}
	if (n == 10 || n == -10)
		c = ft_iten(n);
	if (n < -10)
		c = ft_itn(n);
	else if (n > 10)
		c = ft_itp(n);
	return (c);
}
