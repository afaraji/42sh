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

int			match_file_results(char *ie, int si,
	t_list_head *pr, t_list_head *final_result)
{
	char		*ex;
	int			mf;
	t_list_head	cr;
	t_list_head	rg;

	ttslist_init(&cr);
	ttslist_init(&rg);
	if ((int)ft_strlen(ie) <= si)
		return (merge_ttslist(final_result, pr));
	ex = extract_expression(ie, si, &mf);
	if (ex[0] == 0 && si != 0)
		handle_empty_expression(pr, &cr);
	else
	{
		rg = get_pattern_to_match(ex, NULL, 0,
			ft_strlen(ex));
		if (si == 0)
			handle_first_time_expression(&cr, &rg, mf, ex[0]);
		else
			handle_normal_expression(&cr, &rg, pr, (int[]){ex[si], mf});
	}
	match_file_results(ie, si + ft_strlen(ex) + 1, &cr, final_result);
	match_file_results_free(ex, &rg, pr, &cr);
	return (1);
}

void		expand_element_to_list(t_list_head *main_list,
	t_list_head *expanded_list, t_list_node *expanded_element)
{
	t_list_node *first;
	t_list_node *last;

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

t_list_head	tab_to_list(char **argv)
{
	int			i;
	t_list_head	my_list;

	i = 0;
	ttslist_init(&my_list);
	while (argv[i])
	{
		my_list.push(&my_list, ft_strdup(argv[i]));
		free(argv[i]);
		i++;
	}
	free(argv);
	return (my_list);
}

char		**list_to_tab_oz(t_list_head *arg_list)
{
	char		**argv;
	int			i;
	t_list_node *current_arg;

	i = 0;
	argv = (char **)malloc(sizeof(char *) * (arg_list->size + 1));
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
	int			i;
	t_list_head	final_result;
	char		**result;
	t_list_head arg_list;
	t_list_node *current_arg;

	i = 0;
	arg_list = tab_to_list(argv);
	arg_list.iterator = arg_list.first;
	while ((current_arg = ttslist_iter(&arg_list)))
	{
		if (is_pattern(current_arg->content))
		{
			final_result = pattern_matching(current_arg->content);
			if (final_result.size == 0)
				current_arg->content = unescaped_string(current_arg->content);
			else
				expand_element_to_list(&arg_list, &final_result, current_arg);
		}
		i++;
	}
	result = list_to_tab_oz(&arg_list);
	ttslist_purge(&arg_list, free);
	return (result);
}
