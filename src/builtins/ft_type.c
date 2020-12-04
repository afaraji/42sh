/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_type.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/03 14:35:54 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/04 01:48:50 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/hash_table.h"

static int	output_message(char *arri, char *str, int type)
{
	ft_putstr(arri);
	ft_putstr(" is ");
	if (type == 0)
	{
		ft_putendl("not found");
		return (1);
	}
	else if (type == 1)
	{
		ft_putstr("an alias for ");
		ft_putendl(str);
	}
	else if (type == 2)
		ft_putendl("a shell builtin");
	else if (type == 3)
		ft_putendl(str);
	return (0);
}

int			ft_type(char *cmd, char **arr)
{
	char	*tmp;
	int		ret;
	int		i;

	(void)cmd;
	ret = 0;
	i = 1;
	if (!arr[1])
		return (1);
	tmp = NULL;
	while (arr[i])
	{
		if (ft_strchr(arr[i], '/'))
			output_message(arr[i], arr[i], 3);
		else if ((tmp = get_alias(arr[i])))
			output_message(arr[i], tmp, 1);
		else if (built_in(arr[i]))
			output_message(arr[i], NULL, 2);
		else if ((tmp = hash_chck(arr[i])))
			output_message(arr[i], tmp, 3);
		else
			ret = output_message(arr[i], NULL, 0);
		tmp ? ft_strdel(&tmp) : 0;
		i++;
	}
	return (ret);
}
