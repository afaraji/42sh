/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching_match.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:37:41 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/16 12:52:25 by awali-al         ###   ########.fr       */
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

t_list_head	get_pattern_to_match(char *xprsn, int *index, int i, int xprsn_size)
{
	t_list_head		ranges;
	t_range_match	*rslt;
	unsigned char	escaped;

	i = get_pattern_to_match_init(&ranges, index, &escaped);
	while (++i < xprsn_size)
	{
		rslt = ft_memalloc(sizeof(t_range_match));
		*rslt = (t_range_match){0, 0, MATCH_ONCE, "", 0};
		if (xprsn[i] == '\\' && !escaped && (escaped = 1))
			rslt->error = 1;
		else if (xprsn[i] == '[' && !escaped && !is_quoted(xprsn, i))
			i = handle_opening_bracket(xprsn, i, rslt);
		else if (xprsn[i] == '?' && !escaped && !is_quoted(xprsn, i))
			handle_interrogation_mark(rslt);
		else if (xprsn[i] == '*' && !escaped && !is_quoted(xprsn, i))
			handle_star_mark(rslt);
		else if (xprsn[i] != QUOTE && xprsn[i] != DQUOTE)
			add_character_to_range(rslt, xprsn[i]);
		if (xprsn[i] != '\\' && escaped)
			escaped = 0;
		!rslt->error && xprsn[i] != QUOTE && xprsn[i] != DQUOTE ?
				ranges.push(&ranges, rslt) : free(rslt);
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
