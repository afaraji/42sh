/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:53:32 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/16 12:54:04 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"

void			free_token_node(t_list_token **todel)
{
	ft_strdel(&((*todel)->data));
	free(*todel);
	*todel = NULL;
}
