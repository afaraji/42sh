/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:18:39 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 14:50:17 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pattern_matching.h"

int		match_file_results(char *initial_expression, int start_index,
		t_list_head *past_results, t_list_head *final_result)
{
	char		*expression;
	int			match_folders;
	t_list_head	current_results;
	t_list_head ranges;

	ttslist_init(&current_results);
	ttslist_init(&ranges);
	if (ft_strlen(initial_expression) <= start_index)
		return (merge_ttslist(final_result, past_results));
	expression = extract_expression(initial_expression,
			start_index, &match_folders);
	if (expression[0] == 0 && start_index != 0)
		handle_empty_expression(past_results, &current_results);
	else
	{
		ranges = get_pattern_to_match(expression, NULL, 0);
		if (start_index == 0)
			handle_first_time_expression(&current_results, &ranges,
					match_folders, expression[0]);
		else
			handle_normal_expression(&current_results, &ranges, past_results,
					expression[start_index], match_folders);
	}
	match_file_results(initial_expression, start_index + ft_strlen(expression)
			+ 1, &current_results, final_result);
	free(expression);
	ttslist_purge(&ranges, free);
	ttslist_purge(past_results, free);
	ttslist_purge(&current_results, free);
	return (1);
}

char	*unescaped_string(char *expression)
{
	int		escaped;
	int		i;
	int		result_index;
	char	*result;

	escaped = 0;
	i = 0;
	result_index = 0;
	result = expression;
	while (i < ft_strlen(expression))
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
		i++;
	}
	result[result_index] = '\0';
	return (result);
}

void	pattern_matching(char *expression)
{
	t_list_head current_results;
	t_list_head final_results;
	char        *result;
	
	ttslist_init(&current_results);
	ttslist_init(&final_results);
	match_file_results(expression, 0, &current_results, &final_results);
	if (final_results.size == 0)
		printf("%s", unescaped_string(expression));
	final_results.iterator = final_results.first;
	while ((result = ttslist_iter_content(&final_results)))
	{
		printf("%s ", result);
	}
	printf("\n");
	ttslist_purge(&final_results, free);
}

int		main(int argc, char **argv)
{
	argc = argc + 0;
	pattern_matching(argv[1]);
	return (0);
}
