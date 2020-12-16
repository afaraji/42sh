/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quot_close.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:58:03 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/16 12:59:17 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

static int	is_quote_closed(char *s, int type)
{
	int		i;
	char	c;

	c = (type == DQUOTE) ? '"' : '\'';
	i = 1;
	while (s[i])
	{
		if (s[i] == c && s[i - 1] != '\\')
			return (1);
		i++;
	}
	return (0);
}

int			close_quotes(t_list_token *node)
{
	t_list_token	*ttt;
	char			*toappend;
	char			*tmp;
	char			*tmp2;

	toappend = ft_strjoin(tokentoa(node->type), node->data);
	while (is_quote_closed(toappend, node->type) == 0)
	{
		if (!(tmp = read_to_append(node)))
		{
			toappend ? ft_strdel(&toappend) : 0;
			return (1);
		}
		tmp2 = ft_4strjoin(toappend, "\n", tmp, "");
		toappend ? ft_strdel(&toappend) : 0;
		toappend = tmp2;
		tmp ? ft_strdel(&tmp) : 0;
	}
	ttt = ft_tokenize(toappend);
	replace_node(&node, &ttt);
	toappend ? ft_strdel(&toappend) : 0;
	return (0);
}
