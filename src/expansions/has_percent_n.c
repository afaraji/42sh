/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_percent_n.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 12:53:52 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/11 02:58:23 by awali-al         ###   ########.fr       */
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

int		ft_strstri(const char *haystack, const char *needle)
{
	int		i;
	int		len;
	char	*str;

	str = (char *)haystack;
	if (*needle == '\0')
		return (0);
	i = 0;
	len = ft_strlen(needle);
	while (str[i])
	{
		if ((str[i] == *needle) && (ft_strncmp(&str[i], needle, len) == 0))
			return (i);
		i++;
	}
	return (-1);
}

void	before_and_not_aft(char **trim, char *val, int i)
{
	int		st;
	char	*temp;

	st = 0;
	temp = ft_strndup(*trim, i);
	if (!(*trim)[i + 1])
	{
		st = ft_strstri(val, temp);
		if (st >= 0)
		{
			ft_strdel(trim);
			*trim = ft_strdup(val + st);
		}
	}
	temp ? ft_strdel(&temp) : 0;
}

int		ft_strchri(const char *s, int c)
{
	int		i;
	int		len;
	char	*str;

	str = (char *)s;
	if (c == 0)
	{
		len = ft_strlen(str);
		return (0);
	}
	i = 0;
	while (str[i])
	{
		if (str[i] == (char)c)
			return (i);
		i++;
	}
	return (-1);
}

int		second_case_baf(char **trim, char *val, int i, int ns)
{
	char	*temp1;
	char	*temp2;
	char	*temp3;
	int		find;

	temp1 = ft_strndup(*trim, i);
	temp2 = ft_strdup(*trim + i + ns);
	before_and_aft(&temp1, val, i);
	if ((find = ft_strchri(temp1, temp2[0])))
	{
		temp3 = ft_strndup(temp1, find);
		temp1 ? ft_strdel(&temp1) : 0;
		*trim ? ft_strdel(trim) : 0;
		*trim = ft_strjoin(temp3, temp2);
		temp2 ? ft_strdel(&temp2) : 0;
		temp3 ? ft_strdel(&temp3) : 0;
		return (1);
	}
	temp1 ? ft_strdel(&temp1) : 0;
	temp2 ? ft_strdel(&temp2) : 0;
	return (0);
}

void	trim_end_replace(char **trim, char *val, int *i)
{
	int		st;

	st = 0;
	while ((*trim)[++(*i)])
	{
		if ((*trim)[*i] == '*')
		{
			if ((*i) > 0)
			{
				if (!(*trim)[(*i) + 1])
					before_and_not_aft(trim, val, *i);
				else
				{
					if (before_and_aft(trim, val, *i))
						trim_end_replace(trim, val, i);
				}
			}
			if ((*i) == 0)
				if (only_after(trim, val, i))
					trim_end_replace(trim, val, i);
		}
	}
}
