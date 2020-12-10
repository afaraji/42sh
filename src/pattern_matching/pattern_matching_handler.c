/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching_handler.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:38:33 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 11:22:12 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pattern_matching.h"

int		handle_opening_bracket(char *expression, int i, t_range_match *result)
{
	int	index;

	index = i;
	*result = parse_brackets(expression, &index);
	if (result->error)
	{
		*result = (t_range_match){0, 0, MATCH_ONCE, "", 0};
		add_character_to_range(result, expression[index]);
	}
	return (index);
}

void	handle_interrogation_mark(t_range_match *result)
{
	int	j;

	j = ' ';
	while (j <= '~')
	{
		add_character_to_range(result, j);
		j++;
	}
}

void	handle_star_mark(t_range_match *result)
{
	int	j;

	j = ' ';
	while (j <= '~')
	{
		add_character_to_range(result, j);
		j++;
	}
	result->match_count = MATCH_ZERO_OR_MORE;
}
