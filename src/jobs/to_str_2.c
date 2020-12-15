/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_str_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 20:00:37 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 20:00:50 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"
#include "../../inc/jobs.h"

char	*args_to_str2(t_cmd_suffix *suff, char *s)
{
	char	*str;
	char	*tmp;
	char	*tmp2;

	str = s;
	while (suff)
	{
		if (suff->word)
			tmp = ft_strjoin(str, suff->word);
		else if (suff->io_redirect)
		{
			tmp2 = io_redirect_to_str(suff->io_redirect);
			tmp = ft_strjoin(str, tmp2);
			ft_strdel(&tmp2);
		}
		ft_strdel(&str);
		str = tmp;
		suff = suff->suffix;
	}
	return (str);
}
