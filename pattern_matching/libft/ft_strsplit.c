/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/13 00:29:13 by ozaazaa           #+#    #+#             */
/*   Updated: 2018/10/19 20:50:19 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int		ft_strl(char const *s, char c)
{
	int	i;

	i = 0;
	if (s != NULL)
		while (*(s + i) != c && *(s + i))
			i++;
	return (i);
}

static int		count_word(const char *s, char c)
{
	int	i;
	int	flag;
	int	wr;

	wr = 0;
	i = 0;
	flag = 0;
	while (*(s + i) != '\0' && *(s + i))
	{
		while (*(s + i) == c && *(s + i))
			i++;
		while (*(s + i) != c && *(s + i))
		{
			i++;
			flag = 1;
		}
		if (flag == 1)
		{
			wr++;
			flag = 0;
		}
	}
	return (wr);
}

static char		**ft_strplit(char const *s, char c)
{
	int		i;
	int		wrcount;
	char	**ptr;
	int		j;
	int		k;

	j = 0;
	i = 0;
	wrcount = count_word(s, c);
	if (NULL == (ptr = (char**)malloc(sizeof(char*) * (wrcount + 1))))
		return (NULL);
	while (*(s + i) != '\0' && j < wrcount)
	{
		while (*(s + i) == c && *(s + i))
			i++;
		if (NULL == (ptr[j] = (char*)malloc(sizeof(char) *
						(ft_strl((s + i), c) + 1))))
			return (NULL);
		k = 0;
		while (*(s + (i)) != c && *(s + i))
			ptr[j][k++] = *(s + (i++));
		ptr[j++][k] = '\0';
	}
	ptr[j] = NULL;
	return (ptr);
}

char			**ft_strsplit(char const *s, char c)
{
	if (s == NULL)
		return (NULL);
	else
		return (ft_strplit(s, c));
}
