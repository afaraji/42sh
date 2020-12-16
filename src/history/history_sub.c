/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_sub.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 17:24:50 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/16 17:24:52 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/readline.h"
#include "../../inc/builtins.h"

char	*get_event_disignator(char *s)
{
	int	i;
	int	j;
	int	num;

	i = 0;
	j = 1;
	while (s[i])
	{
		if (s[i] == '!' && (s[i + 1] != '=' && s[i + 1] != '\0') && (i == 0 ||
												(i > 1 && s[i - 1] != '\\')))
		{
			if (s[i + 1] == '!')
				return (ft_strsub(s, i, 2));
			if (s[i + j] == '-')
				j++;
			num = (ft_isdigit(s[i + j])) ? 1 : 0;
			while (s[i + j] && ((!num && ft_isalpha(s[i + j])) ||
												(num && ft_isdigit(s[i + j]))))
				j++;
			return (ft_strsub(s, i, j));
		}
		i++;
	}
	return (NULL);
}

char	*tokens_to_str(t_list_token *node)
{
	char	*s;
	char	*tmp;

	s = ft_strdup("");
	while (node)
	{
		if (node->type == WORD)
			tmp = ft_strjoin(s, node->data);
		else if (node->type == QUOTE || node->type == DQUOTE)
			tmp = ft_4strjoin(s, tokentoa(node->type), node->data,
					tokentoa(node->type));
		else
			tmp = ft_strjoin(s, tokentoa(node->type));
		ft_strdel(&s);
		s = tmp;
		node = node->next;
	}
	return (s);
}

int		history_sub_3(char *old)
{
	ft_print(STDERR, "shell: %s: event not found.\n", old);
	ft_strdel(&old);
	return (1);
}

void	history_sub_2(char *tmp, t_list_token *tokens, t_list_token **node)
{
	t_list_token	*www;
	t_list_token	*zzz;

	www = ft_tokenize(tmp);
	ft_strdel(&tmp);
	zzz = (*node)->next;
	replace_node(node, &www);
	(*node) = tokens;
	while ((*node)->next)
		*node = (*node)->next;
	(*node)->next = zzz;
}

int		history_sub(t_list_token *tokens)
{
	t_list_token	*node;
	char			*new;
	char			*old;
	char			*tmp;

	node = tokens;
	while (node)
	{
		if ((node->type == WORD || node->type == DQUOTE) &&
									(old = get_event_disignator(node->data)))
		{
			free(fc_history_remove());
			if ((new = history_search(old + 1, &g_var.history)) == NULL)
				return (history_sub_3(old));
			tmp = ft_replaceword(node->data, old, new);
			ft_strdel(&old);
			ft_strdel(&new);
			history_sub_2(tmp, tokens, &node);
			tmp = tokens_to_str(tokens);
			fc_history_add(tmp, 1);
			ft_strdel(&tmp);
		}
		node = node->next;
	}
	return (0);
}
