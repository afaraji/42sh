/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:18:39 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 20:08:40 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pattern_matching.h"

int		match_file_results(char *initial_expression, int start_index,
		t_list_head *past_results, t_list_head *final_result)
{
	char		*expression;
	int			match_folders;
	t_list_head	current_results;
	t_list_head ranges;

	ttslist_init(&current_results);
	ttslist_init(&ranges);
	if ((int)ft_strlen(initial_expression) <= start_index)
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
	while (i < (int)ft_strlen(expression))
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

t_list_head	pattern_matching(char *expression)
{
	t_list_head current_results;
	t_list_head final_results;

	ttslist_init(&current_results);
	ttslist_init(&final_results);
	match_file_results(expression, 0, &current_results, &final_results);
	return (final_results);
}



int is_pattern(char *c)
{
	if (ft_strchr(c, '*') || ft_strchr(c, '?') || ft_strchr(c, '['))
		return (1);
	else
	{
		return (0);
	}
}

void expand_element_to_list(t_list_head *main_list, t_list_head *expanded_list, t_list_node *expanded_element)
{
	t_list_node *first;
	t_list_node	*last;

	if (expanded_list->size == 0)
		return ;
	first = expanded_list->first;
	last = expanded_list->last;
	first->prev = expanded_element->prev;
	last->next = expanded_element->next;
	if (expanded_element->prev)
		expanded_element->prev->next = first;
	else
		main_list->first = first;
	if (expanded_element->next)
		expanded_element->next->prev = last;
	else
		main_list->last = last;
	main_list->size = main_list->size + expanded_list->size - 1;
	free(expanded_element->content);
	free(expanded_element);
}

t_list_head tab_to_list(char **argv)
{
	int	i;
	t_list_head my_list;

	i = 0;
	ttslist_init(&my_list);
	while (argv[i])
	{
		my_list.push(&my_list, ft_strdup(argv[i]));
		free(argv[i]);
		i++;
	}
	free(argv);
	return my_list;
}

char** list_to_tab_oz(t_list_head *arg_list)
{
	char	**argv;
	int		i;
	t_list_node *current_arg;

	i = 0;
	argv = (char**)malloc(sizeof(char*) * (arg_list->size + 1));
	arg_list->iterator = arg_list->first;
	while ((current_arg = ttslist_iter(arg_list)))
	{
		argv[i] = ft_strdup(current_arg->content);
		i++;
	}
	argv[i] = NULL;
 	return (argv);
}

char		**expand_pattern(char **argv)
{
	int	i;
	t_list_head final_result;
	char	**result;
	i = 0;

	t_list_head	arg_list;
	t_list_node	*current_arg;

	arg_list = tab_to_list(argv);
	arg_list.iterator = arg_list.first;
	while ((current_arg = ttslist_iter(&arg_list)))
	{
		if (is_pattern(current_arg->content))
		{
			final_result = pattern_matching(current_arg->content);
			if (final_result.size == 0)
			{
				current_arg->content = unescaped_string(current_arg->content);
			}
			else
			{
				expand_element_to_list(&arg_list, &final_result, current_arg);
			}
		}
		i++;
	}
	result = list_to_tab_oz(&arg_list);
	ttslist_purge(&arg_list, free);
	return (result);
}
