/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strshave.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 13:59:25 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/08 14:05:41 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"

int		ft_strshave(char **str, int n)
{
	char	*tmp;

	if (*str)
	{
		tmp = *str;
		*str = ft_strsub(tmp, 0, ft_strlen(tmp) - n);
		ft_strdel(&tmp);
		return (0);
	}
	return (1);
}
