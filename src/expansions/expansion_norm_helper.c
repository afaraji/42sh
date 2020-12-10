/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_norm_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 14:12:17 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/10 20:46:59 by awali-al         ###   ########.fr       */
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

int		norming_shiiiiiiiit(char **argument, char *word, int start)
{
	if ((ft_strlen(ft_strtrim(word)) != ft_strlen(word)) ||
		((*argument)[start + 2] == '?' &&
		(ft_isalnum((*argument)[start + 3]) || (*argument)[start + 3] == '_')))
		return (1);
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
		return (1);
	ft_strdel(argument);
	*argument = ft_strdup(tmp);
	return (0);
}
