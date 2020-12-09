/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_percent_n2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 14:06:10 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/09 14:06:11 by arochdi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"
#include "../../inc/expansion.h"

int		before_and_aft(char **trim, char *val, int i)
{
	int		ns;
	int		end;
	int		st;
	char	*temp;

	st = 0;
	end = 0;
	temp = ft_strndup(*trim, i);
	ns = next_sta((*trim) + i + 1);
	if (!ns)
	{
		st = ft_strstri(val, temp);
		end = ft_strrstr(val, (*trim) + i + 1, ft_strlen((*trim) + i) - 1, i);
		if (end > -1 && st > -1)
		{
			ft_strdel(trim);
			*trim = ft_strndup(val + st, end + 2);
		}
		if (temp)
			ft_strdel(&temp);
		return (0);
	}
	if (temp)
		ft_strdel(&temp);
	return (second_case_baf(trim, val, i, ns));
}

int		only_after(char **trim, char *val, int *i)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strdup(*trim);
	tmp2 = ft_strnew(1);
	tmp2 = ft_strncpy(tmp2, val, 1);
	if (trim)
		ft_strdel(trim);
	if (*trim)
		free(*trim);
	*trim = ft_strdup(ft_strjoin_free(tmp2, tmp, 3));
	(*i)--;
	return (1);
}
