/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 20:38:47 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/03 20:44:58 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"

char	*get_alias(char *str)
{
	t_alias		*tmp;
	char		*ret;

	ret = NULL;
	tmp = g_var.aliases;
	while (tmp && !ret)
	{
		if (!ft_strcmp(str, tmp->key))
			ret = ft_strdup(tmp->sub);
		tmp = tmp->next;
	}
	return (ret);
}

int		is_alias(char *str)
{
	t_alias		*tmp;
	int			ret;

	ret = 0;
	tmp = g_var.aliases;
	while (tmp && !ret)
	{
		if (!ft_strcmp(str, tmp->key))
			ret = 1;
		tmp = tmp->next;
	}
	return (ret);
}
