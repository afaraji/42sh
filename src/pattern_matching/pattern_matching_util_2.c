/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching_util_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 18:35:18 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/14 18:53:49 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pattern_matching.h"

char		*unescaped_string(char *expression)
{
	int		escaped;
	int		i;
	int		result_index;
	char	*result;

	escaped = 0;
	i = -1;
	result_index = 0;
	result = expression;
	while (++i < (int)ft_strlen(expression))
	{
		if (expression[i] == '\\' && !escaped)
			escaped = 1;
		else
		{
			result[result_index] = expression[i];
			result_index++;
		}
		if (escaped > 1)
			escaped = 0;
		if (escaped > 0)
			escaped++;
	}
	result[result_index] = '\0';
	return (result);
}

t_list_head	pattern_matching(char *expression)
{
	t_list_head current_results;
	t_list_head final_results;

	ttslist_init(&current_results);
	ttslist_init(&final_results);
	match_file_results(expression, 0, &current_results, &final_results);
	return (final_results);
}

int			is_pattern(char *c)
{
	if (ft_strchr(c, '*') || ft_strchr(c, '?') || ft_strchr(c, '['))
		return (1);
	else
	{
		return (0);
	}
}

void		swap(char **xp, char **yp)
{
	char *temp;

	temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void		match_file_results_free(char *ex, t_list_head *ranges,
		t_list_head *past_results, t_list_head *cr)
{
	free(ex);
	ttslist_purge(ranges, free);
	ttslist_purge(past_results, free);
	ttslist_purge(cr, free);
}
