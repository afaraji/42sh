/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 15:21:28 by arochdi           #+#    #+#             */
/*   Updated: 2020/11/28 01:46:01 by arochdi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/expansion.h"

int		check_parenth_close(char **argument, int *end, int i, char c)
{
	int		depth;
	int		l;

	l = c == '(' ? i - 2 : i - 1;
	i = l;
	depth = 0;
	while ((*argument)[i])
	{
		if ((*argument)[i] == '$')
			return (-1);
		if ((*argument)[i] == c)
			depth++;
		i++;
	}
	i = l;
	while ((*argument)[i] && depth != 0)
	{
		if ((*argument)[i] == ((c == '(') ? ')' : '}'))
			depth--;
		i++;
	}
	return ((*end = i - 1) && !depth ? (c == '(' ? 1 : 2): -1);
}

int		check_type(char **argument, int *end, int i)
{
	while ((*argument)[++i])
	{
		if ((*argument)[i] == '(' && (*argument)[i + 1] == '(')
		{
			i += 2;
			return (check_parenth_close(argument, end, i, '('));
		}
		else if ((*argument)[i] == '{')
		{
			i++;
			return (check_parenth_close(argument, end, i, '{'));
		}
		else if ((*argument)[i] == '(' && (*argument)[i + 1] != '(')
		{
			while ((*argument)[i] != ')')
				i++;
			return ((*end = i) ? 3 : -1);
		}
		return ((*end = i) ? 0 : -1);
	}
	return (-1);
}

int		juje_dolarate(char **argument, int index)
{
	int		i;

	i = index + 1;
	while ((*argument)[i] && (*argument)[i] != '$' && (*argument)[i] != '~')
		i++;
	return (i);
}

void	ft_expans_replace(char **argument, char *to_change, int start, int end)
{
	*argument = ft_strjoin_free(ft_strndup(*argument, start),
				ft_strjoin_free(to_change,
				ft_strdup(*argument + end), 3), 3);
}

// return 1 : error 
// return 0 : succes
int		expansions_dispatcher(char **argument)
{
	int		i;
	int		end;
	int		c;

	i = -1;
	end = 0;
	while ((*argument)[++i])
	{
		c = -6;
		if ((*argument)[i] == '~' && (*argument)[i + 1] != '$' && i == 0)
			tilde_to_replace(argument, i, ft_strlen(*argument) + 1);
		else if ((*argument)[i] == '~' && (*argument)[i + 1] == '$' && i == 0)
			i = (*argument)[i + 2] ? i : i + 1;
		else if ((*argument)[i] == '$' && (*argument)[i + 1] != '$')
			c = check_type(argument, &end, i);
		if (c == 1)
			i = expans_arithmetic(argument, i, end);
		else if (c == 2)
			i = expans_parameter(argument, i, end - i);
		else if (c == 0)
			i = dollar_replace(argument, i, juje_dolarate(argument, i ? i : 1));
		if (i == -1)
			return (1);
	}
	return (0);
}
