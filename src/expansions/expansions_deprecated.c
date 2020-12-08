/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_deprecated.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <merras@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 18:45:11 by merras            #+#    #+#             */
/*   Updated: 2020/03/05 02:04:09 by arochdi          ###   ########.fr       */
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

static char	*tilde_replace(char *login)
{
	char	*replace;

	replace = NULL;
	if (ft_strlen(login) == 0)
	{
		if (!(replace = var_get_value("HOME", 0)))
			replace = (getpwuid(getuid()))->pw_dir;
	}
	else if (getpwnam(login))
		replace = getpwnam(login)->pw_dir;
	return (replace);
}

static void	tilde_expansion(char **in)
{
	char	login[32];
	char	*replace;
	int		i;

	ft_bzero(login, 32);
	i = 0;
	if ((*in)[0] == '~')
	{
		while ((*in)[++i])
		{
			if ((*in)[i] == '/')
				break ;
			login[i - 1] = (*in)[i];
		}
		if ((replace = tilde_replace(login)))
		{
			replace = ft_strjoin(replace, *in + i);
			free(*in);
			*in = replace;
		}
	}
}

void		dollar_expansion(char **words)
{
	int		i;
	int		temp;
	char	*p;
	char	delimiter;

	i = -1;
	while ((*words)[++i])
	{
		if ((*words)[i] == '$')
		{
			temp = i;
			while (ft_isalnum((*words)[++temp]) || (*words)[temp] == '_')
				continue ;
			delimiter = (*words)[temp];
			(*words)[temp] = 0;
			p = var_get_value(*words + i + 1, 2);
			(*words)[temp] = delimiter;
			(*words)[i] = 0;
			p = ft_strjoin(*words, p);
			p = ft_strjoin_free(p, *words + temp, 1);
			free(*words);
			*words = p;
		}
	}
}

int			dollar_replace(char **argument, int i, int end)
{
	char	*to_change;
	char	*to_change1;
	int		r;

	to_change1 = ft_strndup(*argument + i, end);
	to_change = ft_strdup(str_dollar_sub(to_change1));
	if (!ft_strlen(to_change))
		r = 0;
	else
		r = i + ft_strlen(to_change) - 1;
	ft_print(STDOUT, "--> %s\n", to_change);
	ft_expans_replace(argument, to_change, i, end);
	ft_strdel(&to_change);
	return (r);
}

int			tilde_to_replace(char **argument, int i, int end)
{
	char	*to_change;
	int		r;

	to_change = ft_strndup(*argument + i, end);
	if (!ft_strlen(to_change))
		r = 0;
	else
		r = i + ft_strlen(to_change) - 1;
	tilde_expansion(&to_change);
	ft_expans_replace(argument, to_change, i, end);
	return (r);
}
