/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_percent_expans.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 15:47:47 by arochdi           #+#    #+#             */
/*   Updated: 2020/03/11 15:47:49 by arochdi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/expansion.h"

char	*get_trim_str(char *word, int *percent_pos, char c)
{
	char	*result;
	int		i;

	i = 0;
	while (word[i] && word[i] != c)
		i++;
	*percent_pos = i;
	i = word[i + 1] == c ? i + 1 : i;
	result = ft_strdup(word + i + 1);
	return (result);
}

void	triming_end(char **word, char *trim, int percent_pos, int j)
{
	char	*key;
	char	*word_value;
	int		i;

	key = ft_strndup(*word, percent_pos);
	word_value = var_get_value(key, 2);
	i = ft_strlen(word_value) - 1;
	while (word_value[i] && trim[j])
	{
		if (word_value[i] != trim[j])
		{
			ft_strdel(word);
			*word = ft_strdup(word_value);
			ft_strdel(&key);
			return ;
		}
		i--;
		j--;
	}
	ft_strdel(word);
	*word = (ft_strlen(word_value)) ? ft_strndup(word_value, i + 1) :
			ft_strdup("");
	ft_strdel(&key);
}

void	triming_start(char **word, char *trim, int hash_pos)
{
	char	*key;
	char	*word_value;
	int		i;

	key = ft_strndup(*word, hash_pos);
	word_value = var_get_value(key, 2);
	i = -1;
	while (word_value[++i] && trim[++i])
	{
		if (word_value[i] != trim[i])
		{
			ft_strdel(word);
			*word = ft_strdup(word_value);
			ft_strdel(&key);
			return ;
		}
	}
	ft_strdel(word);
	*word = (ft_strlen(word_value)) ? ft_strdup(word_value + i) :
			ft_strdup("");
	ft_strdel(&key);
}

int		percent_para(char **word)
{
	char	*trim;
	int		percent_pos;

	percent_pos = 0;
	trim = get_trim_str(*word, &percent_pos, '%');
	triming_end(word, trim, percent_pos, ft_strlen(trim) - 1);
	ft_strdel(&trim);
	return (0);
}

int		hash_separator_para(char **word)
{
	char	*trim;
	int		hash_pos;

	hash_pos = 0;
	trim = get_trim_str(*word, &hash_pos, '#');
	triming_start(word, trim, hash_pos);
	ft_strdel(&trim);
	return (0);
}
