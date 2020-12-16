/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_remove_quote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 13:42:31 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/16 13:42:49 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

char			*free_remove_quot(char *s)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (s[i])
	{
		if (s[i] == QUOTE || s[i] == DQUOTE)
		{
			j = i + 1;
			while (s[j] && s[j] != s[i])
				j++;
			if (s[j] != s[i])
				return (s);
			s[i] = '\0';
			s[j] = '\0';
			tmp = ft_4strjoin(s, &s[i + 1], &s[j + 1], "");
			ft_strdel(&s);
			s = tmp;
			i = 2;
		}
		i++;
	}
	return (s);
}
