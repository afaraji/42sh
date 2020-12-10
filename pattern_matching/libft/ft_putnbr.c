/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 18:49:08 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/19 20:45:06 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_printer(long reverse, int n, int a)
{
	int zeros;

	zeros = 0;
	while (n > 0)
	{
		zeros++;
		reverse *= 10;
		reverse += n % 10;
		n /= 10;
	}
	while (reverse > 0)
	{
		n = reverse % 10;
		if ((a == 1) && (reverse / 10 <= 0))
			ft_putchar('8');
		else
			ft_putchar(n + 48);
		zeros--;
		reverse /= 10;
	}
	while (zeros > 0)
	{
		ft_putchar('0');
		zeros--;
	}
}

void		ft_putnbr(int nb)
{
	int n;
	int reverse;
	int a;

	if (nb == -2147483648)
	{
		a = 1;
		nb += 1;
	}
	else
		a = 0;
	n = nb;
	reverse = 0;
	if (nb < 0)
	{
		ft_putchar('-');
		n = nb * -1;
	}
	if (n == 0)
		ft_putchar('0');
	else
		ft_printer(reverse, n, a);
}
