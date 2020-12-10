/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching_evaluate_situation.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:49:22 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 11:11:58 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pattern_matching.h"

int	evaluate_current_situation(t_list_node *range, char *to_match,
		int match_index)
{
	int	len;

	len = ft_strlen(to_match);
	if (!range)
	{
		if (match_index >= len)
			return (SITUATION_STATUS_SUCCESS);
		return (SITUATION_STATUS_FAIL);
	}
	else if (match_index >= len)
		return (SITUATION_STATUS_FAIL);
	return (SITUATION_STATUS_NONE);
}
