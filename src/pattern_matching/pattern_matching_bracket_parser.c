/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching_parser.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:39:38 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 14:28:28 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pattern_matching.h"

t_range_match	parse_bracket_range(char *expression)
{
	t_range_match	result;
	int				escaped;
	int				len;
	int				i;

	i = 0;
	escaped = 0;
	result = (t_range_match){0, 0, MATCH_ONCE, "", 0};
	if (expression[0] == '!' || expression[0] == '^')
	{
		result.except = 1;
		i++;
	}
	len = ft_strlen(expression);
	while (i < len)
	{
		if (expression[i] == '\\' && !escaped)
			escaped = 1;
		else
			handle_range_characters(&result, expression, i, escaped);
		if (expression[i] != '\\')
			escaped = 0;
		i++;
	}
	return (result);
}

t_range_match	parse_brackets(char *expression, int *index)
{
	t_range_match	range_result;
	int				i;
	int				end_index;
	char			*exp;

	end_index = find_bracket_end_index(expression, *index);
	i = *index + 1;
	if (i == end_index)
	{
		range_result.error = 1;
		return (range_result);
	}
	exp = ft_strsub(expression, i, end_index - i);
	range_result = parse_bracket_range(exp);
	*index = end_index;
	free(exp);
	return (range_result);
}

int				find_bracket_end_index(char *expression, int index)
{
	int	i;
	int	escaped;
	int	len;
	int	end_index;

	escaped = 0;
	i = index;
	end_index = i + 1;
	len = ft_strlen(expression);
	while (++i < len)
	{
		if (expression[i] == '\\')
			escaped = 1;
		else if ((expression[i] == ']' || expression[i] == '[') &&
				i != index + 1 + (expression[index + 1] == '!') && !escaped)
		{
			end_index = (expression[i] == ']' ? i : end_index);
			break ;
		}
		if (expression[i] != '\\')
			escaped = 0;
	}
	return (end_index);
}
