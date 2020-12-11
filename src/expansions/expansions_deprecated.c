/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_deprecated.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 20:49:40 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/11 18:23:21 by awali-al         ###   ########.fr       */
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
			in ? ft_strdel(in) : 0;
			*in = replace;
		}
	}
}

int			dollar_replace(char **argument, int i, int end)
{
	char	*to_change;
	int		r;

	to_change = str_dollar_sub(ft_strndup(*argument + i, end));
	if (!ft_strlen(to_change))
		r = 0;
	else
		r = i + ft_strlen(to_change) - 1;
	ft_expans_replace(argument, to_change, i, end);
	to_change ? ft_strdel(&to_change) : 0;
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
	to_change ? ft_strdel(&to_change) : 0;
	return (r);
}
