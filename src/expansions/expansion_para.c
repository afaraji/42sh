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

#include <stdio.h>
#include "../../inc/expansion.h"

int		operation(char **operate, char **word, int check)
{
	if (operate[1][0] == '+')
	{
		ft_strdel(word);
		if (check == 1)
			*word = ft_strdup(operate[1] + 1);
		else
			*word = ft_strdup("");
	}
	else if (operate[1][0] == '-' && check != 1)
	{
		ft_strdel(word);
		*word = ft_strdup(operate[1] + 1);
	}
	else if (operate[1][0] == '?' && check != 1)
		printf("eror\n");
	else if (!(operate[1][0] == '+' || operate[1][0] == '-' ||
				operate[1][0] == '?'))
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
			//set_local_variable(word, i + 1);
			return (-1);
		}
	}
	return (separator_count);
}

int		expans_para(char **word)
{
	int		sep;
	char	*temp;

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
	else if (sep == 0 && (*word)[0] != '#' && !ft_strchr(*word, '#'))
	{
		temp = ft_strjoin_free("$", *word, 2);
		dollar_expansion(&temp);
		*word = temp;
	}
	return (0);
}

int		expans_parameter(char **argument, int start, int end)
{
	char	*word;
	int		r;

	word = ft_strndup(*argument + start + 2, end - 2);
	if (!expans_para(&word))
	{
		if (!ft_strlen(word))
			r = 0;
		else
			r = ft_strlen(word) - 1;
		ft_expans_replace(argument, word, start, end + 1 + start);
	}
	else
		return (-1);
	return (r);
}