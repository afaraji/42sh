/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 17:45:55 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/13 16:27:35 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

char	*ft_strsub_delimit(char *s, char c)
{
	int i;

	i = c;
	i = 0;
	while (s[i] && !ft_isspace(s[i]))
		i++;
	return (ft_strsub(s, 0, i));
}

char	*alias_check(char *s, t_alias *list)
{
	t_alias	*node;

	node = list;
	while (node)
	{
		if (!ft_strcmp(s, node->key))
			return (node->sub);
		node = node->next;
	}
	return (NULL);
}

int		alias_infinit_loop(char *str, char *sub, t_alias *aliases)
{
	char	**t;
	char	*tmp;
	char	*tmp2;
	int		i;
	int		lol;

	t = strsplit_str(sub, "&|;");
	i = 0;
	lol = 0;
	while (t[i] && !lol)
	{
		tmp = ft_strtrim(t[i]);
		if (!ft_strncmp(str, tmp, ft_strlen(str)))
			lol = 1;
		if (!lol && (tmp2 = alias_check(tmp, aliases)) &&
										alias_infinit_loop(str, tmp2, aliases))
			lol = 1;
		ft_strdel(&tmp);
		i++;
	}
	free_tab(t);
	return ((lol) ? 1 : 0);
}

int		alias_sub(t_list_token *word, t_alias *aliases)
{
	t_alias			*node;
	t_list_token	*tmp;

	node = aliases;
	while (node)
	{
		if (!ft_strcmp(word->data, node->key))
		{
			if (!alias_infinit_loop(node->key, node->sub, aliases))
			{
				ft_strdel(&(word->data));
				if (ft_isspace(node->sub[ft_strlen(node->sub) - 1]))
				{
					tmp = word->next;
					while (tmp && tmp->type == SPACE)
						tmp = tmp->next;
					if (tmp && tmp->type == WORD)
						alias_sub(tmp, aliases);
				}
				return ((word->data = ft_strdup(node->sub)) ? 1 : 1);
			}
		}
		node = node->next;
	}
	return (0);
}
