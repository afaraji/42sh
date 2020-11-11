/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strappend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:34:31 by awali-al          #+#    #+#             */
/*   Updated: 2020/11/08 16:39:03 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash_table.h"

char	*ft_strappend(char *str, char c)
{
	char	*ret;
	int		i;

	i = 0;
	ret = ft_strnew(ft_strlen(str) + 1);
	while (str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = c;
	i++;
	ret[i] = '\0';
	return (ret);
}
