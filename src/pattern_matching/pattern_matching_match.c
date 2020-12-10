/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching_match.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:37:41 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 14:29:39 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pattern_matching.h"

t_list_head	get_pattern_to_match(char *expression, int *index, int i)
{
	t_list_head		ranges;
	t_range_match	*result;
	int				expression_size;
	unsigned char	escaped;

	i = -1;
	if (index)
		i = *index - 1;
	escaped = 0;
	expression_size = ft_strlen(expression);
	ttslist_init(&ranges);
	while (++i < expression_size)
	{
		result = ft_memalloc(sizeof(t_range_match));
		*result = (t_range_match){0, 0, MATCH_ONCE, "", 0};
		if (expression[i] == '\\' && !escaped && (escaped = 1))
			result->error = 1;
		else if (expression[i] == '[' && !escaped)
			i = handle_opening_bracket(expression, i, result);
		else if (expression[i] == '?' && !escaped)
			handle_interrogation_mark(result);
		else if (expression[i] == '*' && !escaped)
			handle_star_mark(result);
		else
			add_character_to_range(result, expression[i]);
		if (expression[i] != '\\' && escaped)
			escaped = 0;
		if (!result->error)
			ranges.push(&ranges, result);
	}
	return (ranges);
}

char		*extract_expression(char *initial_expression,
		int start_index, int *match_folders)
{
	int		end_index;

	*match_folders = 0;
	initial_expression = initial_expression + start_index;
	end_index = 0;
	while (initial_expression[end_index])
	{
		if (initial_expression[end_index] == '/')
		{
			*match_folders = 1;
			return (ft_strsub(initial_expression, 0, end_index));
		}
		end_index++;
	}
	return (ft_strdup(initial_expression));
}
