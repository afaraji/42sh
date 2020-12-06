/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_para_utilities.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 15:50:52 by arochdi           #+#    #+#             */
/*   Updated: 2020/03/11 15:50:56 by arochdi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/expansion.h"

#define	PP	':'
#define PPR	'^'

void	clean_shities(char **word)
{
	int		i;

	i = -1;
	while ((*word)[++i])
	{
		if ((*word)[i] == PPR)
			(*word)[i] = PP;
	}
}

void	replace_2p(char **word)
{
	int		i;

	i = -1;
	while ((*word)[++i])
	{
		if ((*word)[i] == PP)
			(*word)[i] = PPR;
	}
}

int		hash_param(char **word)
{
	char	*tmp_a;
	char	*tmp;

	tmp = ft_strdup((*word) + 1);
	tmp_a = ft_strdup(var_get_value(tmp, 2));
	ft_strdel(word);
	*word = ft_itoa(ft_strlen(tmp_a));
	free(tmp_a);
	free(tmp);
	return (0);
}

int		param_is_set(char *var)
{
	char	*value;

	if ((value = var_get_value(var, 2)) && ft_strlen(value) != 0)
		return (1);
	else
		return (0);
}

void	set_local_variable(char **word, int i)
{
	char	**argument;

	argument = (char **)malloc(sizeof(char *) * 4);
	argument[0] = ft_strdup("setenv");
	argument[1] = ft_strndup(*word, i - 1);
	argument[2] = ft_strdup(*word + i + 1);
	argument[3] = NULL;
	ft_strdel(word);
	if (param_is_set(argument[1]))
		*word = ft_strdup(var_get_value(argument[1], 2));
	else
	{
		*word = ft_strdup(argument[2]);
		ft_set_var_env(argument, 1);
	}
	free_tab(argument);
}
