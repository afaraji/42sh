/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_deprecated.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 20:49:40 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/11 18:23:21 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"
#include "../../inc/expansion.h"

int		dollar_replace(char **argument, int i, int end)
{
	char	*to_change;
	int		r;

	to_change = str_dollar_sub(ft_strndup(*argument + i, end));
	if (!ft_strlen(to_change))
		r = 0;
	else
		r = i + ft_strlen(to_change) - 1;
	ft_expans_replace(argument, to_change, i, end);
	to_change ? ft_strdel(&to_change) : 0;
	return (r);
}

int		legal_do(char *word)
{
	int		i;
	char	c;

	i = 0;
	while (word[++i])
	{
		c = word[i];
		if (!ft_isalnum(c) && c != '_' && c != ':' && c != '?'
			&& c != '$' && c != '!')
			return (1);
	}
	return (0);
}
