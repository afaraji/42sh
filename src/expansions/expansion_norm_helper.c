/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_norm_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 14:12:17 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/06 14:12:19 by arochdi          ###   ########.fr       */
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

void	ft_expans_replace(char **argument, char *to_change, int start, int end)
{
	*argument = ft_strjoin_free(ft_strndup(*argument, start),
				ft_strjoin_free(to_change,
				ft_strdup(*argument + end), 3), 3);
}

void	simple_dollar(char **word)
{
	char	*temp;

	temp = ft_strjoin("$", *word);
	ft_strdel(word);
	dollar_expansion(&temp);
	*word = ft_strdup(temp);
}

void	ques_dollar(char **argument)
{
	char	*tmp;

	if (!ft_strcmp("${?}", *argument))
	{
		tmp = str_dollar_sub(*argument);
		ft_strdel(argument);
		*argument = ft_strdup(tmp);
	}
}
