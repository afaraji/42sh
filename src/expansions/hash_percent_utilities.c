/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_percent_utilities.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 14:02:24 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/06 14:02:25 by arochdi          ###   ########.fr       */
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

int		ft_strrstr(const char *haystack, const char *needle,
					size_t len, int lim)
{
	char	*str;
	size_t	j;
	size_t	i;

	str = (char *)haystack;
	if (*needle == '\0')
		return (0);
	i = ft_strlen(haystack) - 1;
	j = 0;
	while (str[i] && (int)i >= lim)
	{
		if (str[i] == needle[0])
		{
			j = 0;
			while (needle[j] && str[i + j] == needle[j] && j < len)
				j++;
			if (needle[j] == '\0')
				return (i);
		}
		i--;
	}
	return (0);
}

int		next_sta(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (i);
		i++;
	}
	return (0);
}

int		find_and_replace(char **trim, char *val, int *i)
{
	char	*s;
	int		l;
	char	*tmp;
	char	*tmp2;

	s = ft_strdup(ft_strrchr((*trim), (*trim)[*i + 1]));
	l = next_sta(s) ? next_sta(s) : ft_strlen(s);
	tmp = ft_strndup(s, l);
	tmp2 = ft_strnew(0);
	if (ft_strrstr(val, tmp, l, *i))
	{
		ft_strdel(&tmp2);
		tmp2 = ft_strndup(val, ft_strrstr(val, tmp, l, *i));
	}
	ft_strdel(trim);
	*trim = ft_strdup(ft_strjoin_free(tmp2, s, 2));
	ft_strdel(&tmp);
	return (0);
}

void	trim_replace(char **trim, char *val, int *i)
{
	if (ft_strequ(*trim, "*"))
	{
		ft_strdel(trim);
		*trim = ft_strdup(val);
		return ;
	}
	while ((*trim)[++(*i)] && val[*i])
	{
		if ((*trim)[*i] == '*' && !(*trim)[*i + 1])
		{
			ft_strdel(trim);
			*trim = ft_strdup(val);
			return ;
		}
		if ((*trim)[*i] == '*' && (*trim)[*i + 1])
		{
			if (find_and_replace(trim, val, i))
				return ;
		}
		else if ((*trim)[*i] != val[*i])
			return ;
		trim_replace(trim, val, i);
	}
}

char	*get_trim_str(char *word, int *percent_pos, char c)
{
	char	*result;
	int		i;

	i = 0;
	while (word[i] && word[i] != c)
		i++;
	*percent_pos = i;
	i = word[i + 1] == c ? i + 1 : i;
	result = ft_strdup(word + i + 1);
	return (result);
}