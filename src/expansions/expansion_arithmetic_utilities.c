/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_arithmetic_utilities.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 02:38:10 by arochdi           #+#    #+#             */
/*   Updated: 2020/11/28 01:45:29 by arochdi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/expansion.h"

static int	num(long long int n)
{
	if (n < 0)
		n *= -1;
	if (n < 10)
		return (1);
	return (1 + num(n / 10));
}

static void	ft_nit_variables(int *c, long long int *nbim, long long int n)
{
	*nbim = n;
	*c = num(n);
}

void		ft_move_space(char **str)
{
	int		i;
	int		l;
	char	*swaped;

	i = -1;
	l = 0;
	while ((*str)[++i])
		if ((*str)[i] == ' ' || (*str)[i] == '\t' || (*str)[i] == '\n')
			l++;
	if (!(swaped = (char *)malloc(sizeof(char) * ft_strlen(*str) - l + 1)))
		return ;
	i = -1;
	l = -1;
	while ((*str)[++i])
		if (!((*str)[i] == ' ' || (*str)[i] == '\t' || (*str)[i] == '\n'))
			swaped[++l] = (*str)[i];
	swaped[l] = 0;
	ft_strdel(str);
	*str = ft_strdup(swaped);
	ft_strdel(&swaped);
}

char		*ft_iitoa(long long int n)
{
	char			*arr;
	int				c;
	long long int	nbim;

	ft_nit_variables(&c, &nbim, n);
	if (n < 0)
	{
		if (!(arr = (char *)malloc(c + 2)))
			return (NULL);
		nbim *= -1;
	}
	else
		arr = (char *)malloc(c + 1);
	arr[c--] = '\0';
	arr[0] = (nbim == 0) ? '0' : arr[0];
	while (nbim != 0)
	{
		arr[c--] = ((nbim % 10) + '0');
		nbim /= 10;
	}
	return (n == (-9223372036854775807 - 1) ?
			ft_strdup("-9223372036854775808") : arr);
}

int			expans_arithmetic(char **argument, int start, int end)
{
	char	*word;
	int		r;

	word = ft_strndup(*argument + start + 3, end - start);
	ft_move_space(&word);
	if (!expans_arith(&word))
	{
		r = ft_strlen(word) - 1;
		ft_expans_replace(argument, word, start, end + 1);
	}
	else
		return (-1);
	return (r);
}
