/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching_handle_range.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:47:44 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 11:22:55 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pattern_matching.h"

void			handle_range_characters(t_range_match *result, char *expression,
		int i, int escaped)
{
	char	before;
	char	after;

	if (i == result->except && (expression[i] == '[' || expression[i] == ']'
				|| expression[i] == '-'))
		add_character_to_range(result, expression[i]);
	else if (expression[i] == '-' && !escaped)
	{
		before = expression[i - 1];
		after = expression[i + 1];
		if (!after)
			add_character_to_range(result, '-');
		else if (before > after)
			result->error = 2;
		else
		{
			while (++before < after + 1)
				add_character_to_range(result, before);
		}
	}
	else
		add_character_to_range(result, expression[i]);
}

void			add_character_to_range(t_range_match *range, char c)
{
	if (ft_strchr(range->characters, c) == NULL)
	{
		range->characters[range->buffer_size] = c;
		range->buffer_size++;
		range->characters[range->buffer_size] = 0;
	}
}

unsigned char	match_strings_to_range(char *to_match, t_list_node *range,
		int match_index)
{
	int				status;
	t_range_match	*current_range;
	char			current_char;

	status = evaluate_current_situation(range, to_match, match_index);
	if (status)
		return (status > 0 ? 1 : 0);
	current_range = range->content;
	current_char = to_match[match_index];
	if (range_matches_character(current_char, current_range))
	{
		if (current_range->match_count == MATCH_ZERO_OR_MORE)
		{
			if (match_strings_to_range(to_match, range, match_index + 1) ||
					match_strings_to_range(to_match, range->next,
						match_index + 1) || match_strings_to_range(to_match,
							range->next, match_index))
				return (1);
		}
		else
			return (match_strings_to_range(to_match, range->next,
						match_index + 1));
	}
	return (0);
}

unsigned char	range_matches_character(char current_char,
		t_range_match *current_range)
{
	int	i;

	i = 0;
	while (i < current_range->buffer_size)
	{
		if (current_char == current_range->characters[i])
		{
			if (current_range->except)
				return (0);
			return (1);
		}
		i++;
	}
	return (current_range->except);
}
