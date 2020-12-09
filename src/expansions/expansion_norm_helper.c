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
	char	*first;
	char	*second;

	first = ft_strndup(*argument, start);
	second = ft_strdup(*argument + end);
	*argument = ft_strjoin_free(first, ft_strjoin_free(to_change,
				second, 2), 1);
}

void	simple_dollar(char **word)
{
	char	*temp;

	temp = ft_strjoin("$", *word);
	ft_strdel(word);
	dollar_expansion(&temp);
	*word = ft_strdup(temp);
}

int		ilegal_do(char *word, char c)
{
	int		i;

	i = 0;
	if (word[i] != c || word[i] == '+' || word[i] == '-' || word[i] == '%'
		|| word[i] == '*' || word[i] == '|' || word[i] == '\\'
		|| word[i] == '/')
		return (0);
	while (word[++i])
	{
		if (ft_isalnum(word[i]) || word[i] == c || word[i] == '_')
			return (0);
		else
			word[i] = '\0';
	}
	return (1);
}

void	ques_dollar(char **argument)
{
	char	*tmp;

	tmp = str_dollar_sub(ft_strjoin("$", *argument));
	ft_strdel(argument);
	*argument = ft_strdup(tmp);
}
