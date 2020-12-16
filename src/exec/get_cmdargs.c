/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmdargs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 18:19:11 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/16 13:02:23 by awali-al         ###   ########.fr       */
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
#include "../../inc/pattern_matching.h"

t_l		*fill_get_args(char *str)
{
	t_l	*node;

	node = (t_l *)malloc(sizeof(t_l));
	if (!node)
		return (NULL);
	node->data = ft_strdup(str);
	node->next = NULL;
	return (node);
}

t_l		*get_args(t_simple_cmd *cmd)
{
	t_l				*head;
	t_l				*node;
	t_cmd_suffix	*tmp;

	if (!(cmd->name) && !(cmd->word))
		return (NULL);
	if (!(head = (t_l *)malloc(sizeof(t_l))))
		return (NULL);
	if (cmd->name)
		head->data = ft_strdup(cmd->name);
	else
		head->data = ft_strdup(cmd->word);
	head->next = NULL;
	node = head;
	if (cmd->suffix)
	{
		tmp = cmd->suffix;
		while (tmp)
		{
			if (tmp->word && (node->next = fill_get_args(tmp->word)))
				node = node->next;
			tmp = tmp->suffix;
		}
	}
	return (head);
}

int		is_builtin(char *str)
{
	char	**b_in_list;
	int		i;
	char	*tmp;

	tmp = ft_strjoin(BUILTINS, BUILTINS2);
	b_in_list = ft_strsplit(tmp, '|');
	ft_strdel(&tmp);
	if (!str || !ft_strcmp(str, ""))
	{
		free_tab(b_in_list);
		return (0);
	}
	i = 0;
	while (b_in_list[i])
	{
		if (!ft_strcmp(b_in_list[i], str))
		{
			free_tab(b_in_list);
			return (i + 1);
		}
		i++;
	}
	free_tab(b_in_list);
	return (0);
}

void	push_to_list(t_l **head, char *s)
{
	t_l		*node;

	if (*head == NULL)
	{
		*head = fill_get_args(s);
		ft_strdel(&s);
		return ;
	}
	node = *head;
	while (node->next)
		node = node->next;
	node->next = fill_get_args(s);
	ft_strdel(&s);
}

char	**split_expanssion(char *s)
{
	t_l		*list;
	int		i;
	int		quoted;
	int		j;

	i = 0;
	j = 0;
	quoted = 0;
	list = NULL;
	while (s[i])
	{
		if (s[i] == QUOTE || s[i] == DQUOTE)
			quoted = (quoted == 0) ? 1 : 0;
		else if (quoted == 0 && (s[i] == ' ' || s[i] == '\n' || s[i] == '\t')
												&& (i == 0 || s[i - 1] != '\\'))
		{
			push_to_list(&list, ft_strsub(s, 0, i));
			s = s + i + 1;
			i = -1;
		}
		i++;
	}
	push_to_list(&list, ft_strsub(s, 0, i));
	return (list_to_tab(list));
}

t_l		*var_sub(t_l *head)
{
	t_l		*node;
	t_l		*next_node;
	char	**t;
	int		i;

	next_node = head->next;
	if (expansions_dispatcher(&(head->data)))
	{
		g_var.errno = 11;
		return (head);
	}
	t = split_expanssion(head->data);
	ft_strdel(&(head->data));
	i = 1;
	if (!t || !t[0])
	{
		head->data = ft_strdup("");
		head->next = next_node;
		return (head);
	}
	head->data = ft_strdup(t[0]);
	node = head;
	while (t[i])
	{
		node->next = fill_get_args(t[i]);
		node = node->next;
		i++;
	}
	free_tab(t);
	node->next = next_node;
	return (head);
}

int		param_expand(t_l *list)
{
	t_l		*node;

	node = list;
	while (node)
	{
		node = var_sub(node);
		if (g_var.errno)
		{
			free_l(list);
			return (1);
		}
		node = node->next;
	}
	return (0);
}

char	**quote_removal(char **av)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	while (av[i])
	{
		av[i] = free_remove_quot(av[i]);
		tmp2 = ft_replaceword(av[i], "\\\\", "\375");
		tmp = ft_replaceword(tmp2, "\\", "");
		ft_strdel(&tmp2);
		tmp2 = ft_replaceword(tmp, "\375", "\\");
		ft_strdel(&tmp);
		free(av[i]);
		av[i] = tmp2;
		i++;
	}
	return (av);
}

char	**get_arg_var_sub(t_simple_cmd *cmd)
{
	t_l		*list;
	char	**table;

	if ((list = get_args(cmd)) == NULL)
		return (NULL);
	if (param_expand(list))
		return (NULL);
	if ((table = list_to_tab(list)) == NULL)
		return (NULL);
	table = expand_pattern(table);
	table = quote_removal(table);
	return (table);
}

/*
** Shell Parameter Expansion
** Command Substitution
** Arithmetic Expansion
** Word Splitting
** Filename Expansion
** Quote Removal
*/
