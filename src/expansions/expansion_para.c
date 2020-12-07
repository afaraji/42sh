/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_para.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abiri <abiri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 16:31:12 by arochdi           #+#    #+#             */
/*   Updated: 2020/11/28 01:45:46 by arochdi          ###   ########.fr       */
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

int		operation(char **operate, char **word, int check)
{
	if (operate[1][0] == '+')
	{
		ft_strdel(word);
		if (check == 1)
			*word = ft_strdup(ft_strtrim(operate[1] + 1));
		else
			*word = ft_strdup("");
	}
	else if (operate[1][0] == '-' && check != 1)
	{
		ft_strdel(word);
		*word = ft_strdup(ft_strtrim(operate[1] + 1));
	}
	else if (operate[1][0] == '?' && check != 1)
		return (ft_print(STDOUT, "42sh: %s: %s\n", operate[0], operate[1] + 1));
	else if (!(operate[1][0] == '+' || operate[1][0] == '-' ||
			operate[1][0] == '?' || ft_isalnum(operate[1][0]) ||
			ft_isspace(operate[1][0])))
		return (1);
	else if (check == 1)
	{
		ft_strdel(word);
		*word = ft_strdup(var_get_value(operate[0], 2));
	}
	return (0);
}

int		normal_case(char **word)
{
	char	**operate;
	int		check;
	int		ret;

	operate = ft_strsplit(*word, ':');
	check = param_is_set(operate[0]);
	if (dollared(operate[0]))
		return (1);
	if (operate[1] && dollared(operate[1]))
		dollar_expansion(&(operate[1]));
	ret = operate[1] ? operation(operate, word, check) : 1;
	free_tab(operate);
	return (ret);
}

int		sep_count(char **word)
{
	int		separator_count;
	int		i;

	i = -1;
	separator_count = 0;
	while ((*word)[++i])
	{
		if ((*word)[i] == ':' && (*word)[i + 1] != '=')
			separator_count++;
		else if ((*word)[i] == ':' && (*word)[i + 1] == '=')
		{
			if (dollared(ft_strsub((*word), 0, i)))
				return (0);
			if (dollared((*word) + i + 2))
				dollar_expansion(word);
			set_local_variable(word, i + 1);
			return (-1);
		}
	}
	return (separator_count);
}

int		expans_para(char **word)
{
	int		sep;

	sep = sep_count(word);
	if (sep == 1 && (*word)[0] != '#' && ft_strlen(*word) > 1)
		return (normal_case(word));
	else if (sep == 0 && (*word)[0] == '#')
		return (hash_param(word));
	else if ((*word)[0] != '#' && (*word)[0] != '%' &&
			(ft_strchr(*word, '%') || ft_strstr(*word, "%%")))
		return (percent_para(word));
	else if ((*word)[0] != '#' && (*word)[0] != '%' &&
			(ft_strchr(*word, '#') || ft_strstr(*word, "##")))
		return (hash_separator_para(word));
	else if (sep > 1)
		return (1);
	else if (sep == 0 && dollared(*word))
		return (1);
	else if (sep == 0 && (*word)[0] != '#' && !ft_strchr(*word, '#'))
		simple_dollar(word);
	return (0);
}

int		expans_parameter(char **argument, int start, int end)
{
	char	*word;
	int		r;

	word = ft_strndup(*argument + start + 2, end - 2);
	if (ft_strlen(ft_strtrim(word)) != ft_strlen(word))
	{
		ft_strdel(&word);
		return (-1);
	}
	if (!expans_para(&word))
	{
		clean_shities(&word);
		if (!ft_strlen(word))
			r = 0;
		else
			r = ft_strlen(word) - 1;
		ft_expans_replace(argument, word, start, end + 1 + start);
	}
	else
	{
		ft_strdel(&word);
		return (-1);
	}
	return (r);
}
