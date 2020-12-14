/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_percent_expans.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 15:47:47 by arochdi           #+#    #+#             */
/*   Updated: 2020/12/11 17:55:43 by awali-al         ###   ########.fr       */
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

void	triming_end_helper(char **word, char **trim,
							int percent_pos, char **word_value)
{
	char	*key;
	int		i;

	key = ft_strndup(*word, percent_pos);
	*word_value = ft_strdup(var_get_value(key, 2));
	i = -1;
	if (ft_strlen(*word_value))
		trim_end_replace(trim, *word_value, &i);
	key ? ft_strdel(&key) : 0;
}

void	triming_end(char **word, char *trim, int percent_pos)
{
	char	*word_value;
	int		i;
	int		j;

	triming_end_helper(word, &trim, percent_pos, &word_value);
	j = ft_strlen(trim) - 1;
	i = ft_strlen(word_value) - 1;
	while (word_value[i] && trim[j] && j >= 0)
	{
		if (word_value[i] != trim[j])
		{
			*word ? ft_strdel(word) : 0;
			*word = ft_strdup(word_value);
			word_value ? ft_strdel(&word_value) : 0;
			return ;
		}
		i--;
		j--;
	}
	*word ? ft_strdel(word) : 0;
	*word = (ft_strlen(word_value)) ? ft_strndup(word_value, i + 1) :
			ft_strdup("");
	ft_strdel(&word_value);
	ft_strdel(&trim);
}

void	triming_start(char **word, char *trim, int hash_pos)
{
	char	*key;
	char	*word_value;
	int		i;

	key = ft_strndup(*word, hash_pos);
	word_value = var_get_value(key, 2);
	i = -1;
	if (ft_strlen(word_value))
		trim_replace(&trim, word_value, &i);
	i = -1;
	while (word_value[++i] && trim[i])
	{
		if (word_value[i] != trim[i])
		{
			ft_strdel(word);
			*word = ft_strdup(word_value);
			key ? ft_strdel(&key) : 0;
			return ;
		}
	}
	*word ? ft_strdel(word) : 0;
	*word = (ft_strlen(word_value)) ? ft_strdup(word_value + i) :
			ft_strdup("");
	key ? ft_strdel(&key) : 0;
	trim ? ft_strdel(&trim) : 0;
}

int		percent_para(char **word)
{
	char	*trim;
	int		percent_pos;

	clean_shities(word);
	percent_pos = 0;
	trim = get_trim_str(*word, &percent_pos, '%');
	if (dollared(trim))
		dollar_replace(&trim, 0, ft_strlen(trim));
	clean_shities(word);
	trim = free_remove_quot(trim);
	triming_end(word, trim, percent_pos);
	return (0);
}

int		hash_separator_para(char **word)
{
	char	*trim;
	int		hash_pos;

	clean_shities(word);
	hash_pos = 0;
	trim = get_trim_str(*word, &hash_pos, '#');
	if (dollared(trim))
		dollar_replace(&trim, 0, ft_strlen(trim));
	triming_start(word, trim, hash_pos);
	return (0);
}
