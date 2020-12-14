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

int			is_quoted(char *str, int index)
{
	int	flag;
	int	i;

	i = 0;
	flag = 0;
	while (str[i] != '\0')
	{
		if ((str[i] == QUOTE || str[i] == DQUOTE) && flag == 0)
			flag = 1;
		else if ((str[i] == QUOTE || str[i] == DQUOTE) && flag == 1)
			flag = 0;
		else if (index == i && flag == 1)
			return (1);
		i++;
	}
	return (0);
}

int			get_pattern_to_match_init(t_list_head *ranges, int *index,
				unsigned char *escaped)
{
	int	i;

	i = index ? *index - 1 : -1;
	*escaped = 0;
	ttslist_init(ranges);
	return (i);
}

t_list_head	get_pattern_to_match(char *expression, int *index, int i,
					int expression_size)
{
	t_list_head		ranges;
	t_range_match	*result;
	unsigned char	escaped;

	i = get_pattern_to_match_init(&ranges, index, &escaped);
	while (++i < expression_size)
	{
		result = ft_memalloc(sizeof(t_range_match));
		*result = (t_range_match){0, 0, MATCH_ONCE, "", 0};
		if (expression[i] == '\\' && !escaped && (escaped = 1))
			result->error = 1;
		else if (expression[i] == '[' && !escaped && !is_quoted(expression, i))
			i = handle_opening_bracket(expression, i, result);
		else if (expression[i] == '?' && !escaped && !is_quoted(expression, i))
			handle_interrogation_mark(result);
		else if (expression[i] == '*' && !escaped && !is_quoted(expression, i))
			handle_star_mark(result);
		else if (expression[i] != QUOTE && expression[i] != DQUOTE)
			add_character_to_range(result, expression[i]);
		if (expression[i] != '\\' && escaped)
			escaped = 0;
		if (!result->error && expression[i] != QUOTE && expression[i] != DQUOTE)
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
