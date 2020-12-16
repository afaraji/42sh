/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_name.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 13:43:58 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/16 13:54:12 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/parse.h"

char			*cmd_name(t_list_token **cmd, t_list_token **end)
{
	char	*name;

	while (*cmd && (*cmd)->type == SPACE && *cmd != *end)
		*cmd = (*cmd)->next;
	if (!cmd || !(*cmd) || g_var.errno)
		return (NULL);
	if (((*cmd)->type == WORD || (*cmd)->type == QUOTE ||
		(*cmd)->type == DQUOTE))
	{
		name = ft_strdup((*cmd)->data);
		*cmd = (*cmd)->next;
		return (name);
	}
	return (NULL);
}
