/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_norm_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 14:12:17 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/13 17:29:50 by awali-al         ###   ########.fr       */
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

int		norming_shiiiiiiiit(char **arg, char **word, int start)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strtrim(*word);
	if ((ft_strlen(tmp) != ft_strlen(*word)) || ((*arg)[start + 2] == '?' &&
		(ft_isalnum((*arg)[start + 3]) || (*arg)[start + 3] == '_')))
	{
		tmp ? ft_strdel(&tmp) : 0;
		return (1);
	}
	if (ft_strchri(tmp, '?') > 0)
	{
		tmp2 = ft_strndup(tmp, ft_strchri(tmp, '?'));
		if (!param_is_set(tmp2))
		{
			tmp ? ft_strdel(&tmp) : 0;
			tmp2 ? ft_strdel(&tmp2) : 0;
			return (1);
		}
		*word ? ft_strdel(word) : 0;
		*word = ft_strdup(tmp2);
		tmp2 ? ft_strdel(&tmp2) : 0;
	}
	tmp ? ft_strdel(&tmp) : 0;
	return (0);
}

void	ft_expans_replace(char **argument, char *to_change, int start, int end)
{
	char	*first;
	char	*second;

	first = ft_strndup(*argument, start);
	second = ft_strjoin(to_change, *argument + end);
	*argument = ft_strjoin(first, second);
	first ? ft_strdel(&first) : 0;
	second ? ft_strdel(&second) : 0;
}

int		ilegal_do(char *word, char c)
{
	int		i;

	i = 0;
	while (word[++i])
	{
		if (ft_isalnum(word[i]) || word[i] == c || word[i] == '_')
			return (0);
		else
			word[i] = '\0';
	}
	return (1);
}

int		ques_dollar(char **argument)
{
	char	*tmp;

	tmp = str_dollar_sub(ft_strjoin("$", *argument));
	if ((*argument)[0] == '+' || (*argument)[0] == '-' || (*argument)[0] == '%'
		|| (*argument)[0] == '*' || (*argument)[0] == '|'
		|| (*argument)[0] == '\\' || (*argument)[0] == '/')
	{
		tmp ? ft_strdel(&tmp) : 0;
		return (1);
	}
	ft_strdel(argument);
	*argument = tmp;
	return (0);
}
