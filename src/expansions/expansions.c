/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 15:21:28 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/11 17:14:22 by awali-al         ###   ########.fr       */
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

int		check_parenth_close(char **arg, int *end, int i, char c)
{
	int		depth;
	int		l;

	l = c == '(' ? i - 2 : i - 1;
	i = l;
	depth = 0;
	while ((*arg)[i] && (*arg)[i] != '$')
	{
		if ((*arg)[i] == c)
			depth++;
		i++;
	}
	i = l;
	while ((*arg)[i] && depth != 0)
	{
		if ((*arg)[i] == ((c == '(') ? ')' : '}') && (*arg)[i - 1] != '\\')
			depth--;
		i++;
	}
	if ((*end = i - 1) && !depth && c == '(')
		return (1);
	else if ((*end = i - 1) && !depth && c != '(')
		return (2);
	else
		return (-exp_err("Shell : syntax error"));
}

int		check_type(char **argument, int *end, int i)
{
	while ((*argument)[++i])
	{
		if ((*argument)[i] == '(' && (*argument)[i + 1] == '(')
		{
			i += 2;
			return (check_parenth_close(argument, end, i, '('));
		}
		else if ((*argument)[i] == '{')
		{
			i++;
			return (check_parenth_close(argument, end, i, '{'));
		}
		else if ((*argument)[i] == '(' && (*argument)[i + 1] != '(')
		{
			while ((*argument)[i] != ')')
				i++;
			return ((*end = i) ? 3 : -1);
		}
		return ((*end = i) ? 0 : -1);
	}
	return (-1);
}

int		juje_dolarate(char **argument, int index)
{
	int		i;

	i = index + 1;
	while ((*argument)[i] && (*argument)[i] != '$' && (*argument)[i] != '~')
		i++;
	return (i);
}

int		expansions_dispatcher(char **argument)
{
	int		i;
	int		end;
	int		c;

	i = -1;
	while ((*argument)[++i])
	{
		c = -6;
		if ((*argument)[i] == '~' && (*argument)[i + 1] == '$' && i == 0)
			i = (*argument)[i + 2] ? i : i + 1;
		else if ((*argument)[i] == '$' && (*argument)[i + 1] != '$')
			c = check_type(argument, &end, i);
		if (c == 1)
			i = expans_arithmetic(argument, i, end);
		else if (c == 2)
			i = expans_parameter(argument, i, end - i);
		else if (c == 0 || dollared((*argument)))
			i = dollar_replace(argument, i, ft_strlen(*argument));
		if (i == -1 || c == -1)
			return (1);
	}
	return (0);
}

int		expansions(t_list_token *tokens)
{
	t_list_token	*node;
	int				ret;

	node = tokens;
	ret = 0;
	while (node)
	{
		if (node->type == WORD)
			ret = expansions_dispatcher(&node->data);
		if (ret)
			return (ret);
		node = node->next;
	}
	return (0);
}
