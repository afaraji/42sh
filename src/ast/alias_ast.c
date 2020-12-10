/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 17:45:55 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/10 19:00:49 by awali-al         ###   ########.fr       */
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

// int		alias_infinit_loop(char *str, t_alias *aliases)
// {
// 	char	*tmp;
// 	t_alias	*node;

// 	node = aliases;
// 	while (node)
// 	{
// 		if (!ft_strcmp(str, node->key))
// 		{
// 			tmp = node->sub;
// 			alias_check(aliases, str, node->sub);
// 		}
// 		node = node->next;
// 	}
// 	ft_strdel(&tmp);
// 	return (0);
// }

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
		if (!ft_strcmp(str, tmp))
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

// int		alias_infinit_loop(char *str, t_alias *aliases)
// {
// 	char	*tmp;
// 	t_alias	*node;

// 	node = aliases;
// 	tmp = ft_strdup(str);
// 	while (node)
// 	{
// 		if (!ft_strcmp(tmp, node->key))
// 		{
// 			ft_strdel(&tmp);
// 			tmp = ft_strsub_delimit(node->sub, ' ');
// 			if (!ft_strcmp(tmp, str))
// 			{
// 				ft_strdel(&tmp);
// 				return (1);
// 			}
// 			else
// 				node = aliases;
// 		}
// 		node = node->next;
// 	}
// 	ft_strdel(&tmp);
// 	return (0);
// }

int		alias_sub(t_list_token *word, t_alias *aliases)
{
	t_alias	*node;

	node = aliases;
	while (node)
	{
		if (!ft_strcmp(word->data, node->key))
		{
			if (!alias_infinit_loop(node->key, node->sub, aliases))
			{
				ft_strdel(&(word->data));
				word->data = ft_strdup(node->sub);
				return (1);
			}
		}
		node = node->next;
	}
	return (0);
}
