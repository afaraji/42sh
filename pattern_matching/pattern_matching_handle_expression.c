/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expression.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:32:46 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 14:00:54 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pattern_matching.h"

void	handle_empty_expression(t_list_head *past_result,
		t_list_head *current_result)
{
	char	*content;

	past_result->iterator = past_result->first;
	while ((content = ttslist_iter_content(past_result)))
		current_result->push(past_result, ft_strjoin(content, "/"));
}

int		handle_first_time_expression(t_list_head *current_results,
		t_list_head *ranges, int match_folders, char first_char)
{
	t_list_head	current_candidates;
	char		*candidate;

	ttslist_init(&current_candidates);
	if (first_char == 0)
		current_results->push(current_results, ft_strdup("/"));
	current_candidates = get_file_names(".", (first_char == '.'));
	current_candidates.iterator = current_candidates.first;
	while ((candidate = ttslist_iter_content(&current_candidates)))
	{
		if (!(match_folders && !is_dir(candidate)))
		{
			if (match_strings_to_range(candidate, ranges->first, 0))
			{
				current_results->push(current_results, ft_strjoin(candidate,
							(match_folders ? "/" : "")));
			}
		}
	}
	ttslist_purge(&current_candidates, free);
	return (0);
}

int		handle_normal_expression(t_list_head *current_results,
		t_list_head *ranges, t_list_head *past_results, char first_char,
		int match_folders)
{
	t_list_head	current_candidates;
	char		*past;
	char		*candidate;
	char		*empty;

	ttslist_init(&current_candidates);
	past_results->iterator = past_results->first;
	while ((past = ttslist_iter_content(past_results)))
	{
		current_candidates = get_file_names(past, (first_char == '.'));
		current_candidates.iterator = current_candidates.first;
		while ((candidate = ttslist_iter_content(&current_candidates)))
		{
			empty = ft_strjoin(past, candidate);
			if (!(match_folders && !is_dir(empty)))
			{
				if (match_strings_to_range(candidate, ranges->first, 0))
					current_results->push(current_results, ft_strjoin_free(past,
								ft_strjoin(candidate,
									(match_folders ? "/" : "")), 2));
			}
			free(empty);
		}
	}
	ttslist_purge(&current_candidates, free);
	return (0);
}
