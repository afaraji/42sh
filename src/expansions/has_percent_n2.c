/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_percent_n2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 14:06:10 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/11 17:53:18 by awali-al         ###   ########.fr       */
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

int		operate_leg(char *oprate)
{
	int		l;

	l = ft_strlen(oprate);
	if (oprate[l - 1] == '+' || oprate[l - 1] == '-' ||
			oprate[l - 1] == '?' || ft_isspace(oprate[l - 1]))
		return (1);
	return (0);
}

void	nor_s(char **operate, char **word, int check)
{
	char	*tmp;

	tmp = ft_strtrim(operate[1] + 1);
	if (operate[1][0] == '+')
	{
		ft_strdel(word);
		if (check == 1)
			*word = ft_strdup(tmp);
		else
			*word = ft_strdup("");
	}
	else if (operate[1][0] == '-' && check != 1)
	{
		ft_strdel(word);
		*word = ft_strdup(tmp);
	}
	tmp ? ft_strdel(&tmp) : 0;
}

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
		temp ? ft_strdel(&temp) : 0;
		return (0);
	}
	temp ? ft_strdel(&temp) : 0;
	return (second_case_baf(trim, val, i, ns));
}

int		only_after(char **trim, char *val, int *i)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strdup(*trim);
	tmp2 = ft_strnew(1);
	tmp2 = ft_strncpy(tmp2, val, 1);
	*trim ? ft_strdel(trim) : 0;
	*trim = ft_strjoin(tmp2, tmp);
	tmp ? ft_strdel(&tmp) : 0;
	tmp2 ? ft_strdel(&tmp2) : 0;
	(*i)--;
	return (1);
}
