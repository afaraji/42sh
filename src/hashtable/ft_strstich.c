/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstich.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 17:52:59 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/03 00:11:16 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/hash_table.h"

char	*ft_strstich(char *str1, char thread, char *str2)
{
	char	*ret;
	char	*tmp;

	tmp = ft_strappend(str1, thread);
	ret = ft_strjoin(tmp, str2);
	ft_strdel(&tmp);
	return (ret);
}
