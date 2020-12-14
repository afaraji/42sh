/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_para.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 16:31:12 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/13 20:17:51 by awali-al         ###   ########.fr       */
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

int		operation(char **oprate, char **word, int check)
{
	if (oprate[1][0] == '+' || (oprate[1][0] == '-' && check != 1))
		nor_s(oprate, word, check);
	else if (oprate[1][0] == '?' && check != 1)
		return (ft_print(STDOUT, "Shell: %s: %s\n", oprate[0], oprate[1] + 1));
	else if (!(oprate[1][0] == '+' || oprate[1][0] == '-' ||
			oprate[1][0] == '?' || ft_isalnum(oprate[1][0]) ||
			ft_isspace(oprate[1][0])))
		return (exp_err("Shell: Bad substitution"));
	else if (operate_leg(oprate[0]))
		return (exp_err("Shell: Bad substitution"));
	else if (check == 1)
	{
		ft_strdel(word);
		*word = ft_strdup(var_get_value(oprate[0], 2));
	}
	return (0);
}

int		normal_case(char **word)
{
	char	**operate;
	int		check;
	int		ret;

	operate = ft_strsplit(*word, ':');
	if (operate[1] && (ft_isalnum(operate[1][0]) || operate[1][0] == '_'
		|| operate[1][0] == '*' || operate[1][0] == '|'
		|| operate[1][0] == '\\' || operate[1][0] == '/'
		|| operate[1][0] == '$') && !operate_leg(operate[0]))
	{
		ft_strdel(word);
		ques_dollar(&(operate[0]));
		*word = ft_strdup(operate[0]);
		free_tab(operate);
		return (0);
	}
	check = param_is_set(operate[0]);
	if (dollared(operate[0]))
		return (exp_err("Shell: Bad substitution"));
	if (operate[1] && dollared(operate[1]))
		ques_dollar(&(operate[1]));
	ret = operate[1] ? operation(operate, word, check) : 1;
	free_tab(operate);
	return (ret);
}

int		sep_count(char **word)
{
	char	*tmp;
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
			tmp = ft_strsub((*word), 0, i);
			if (dollared(tmp))
			{
				tmp ? ft_strdel(&tmp) : 0;
				return (0);
			}
			tmp ? ft_strdel(&tmp) : 0;
			dollared((*word) + i + 2) ? ques_dollar(word) : 0;
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
		return (exp_err("Shell: Bad substitution"));
	else if (sep == 0 && (dollared(*word) && !ilegal_do(*word, '$')))
		return (exp_err("Shell: Bad substitution"));
	else if (sep == 0 && (*word)[0] != '#' && !ft_strchr(*word, '#'))
		return (ques_dollar(word));
	return (0);
}

int		expans_parameter(char **argument, int start, int end)
{
	char	*word;
	int		r;

	r = 0;
	word = (*argument)[start + 2] == '?' &&
		(!ft_isalnum((*argument)[start + 3]) || (*argument)[start + 3] != '_') ?
		ft_strdup("?") : ft_strndup(*argument + start + 2, end - 2);
	if (norming_shiiiiiiiit(argument, &word, start))
	{
		word ? ft_strdel(&word) : 0;
		return (-1);
	}
	if (!expans_para(&word))
	{
		clean_shities(&word);
		r = ft_strlen(word) ? ft_strlen(word) - 1 : 0;
		ft_expans_replace(argument, word, start, end + 1 + start);
		word ? ft_strdel(&word) : 0;
	}
	else
	{
		word ? ft_strdel(&word) : 0;
		return (-1);
	}
	return (r);
}
