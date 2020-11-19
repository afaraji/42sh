/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables_completion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 19:33:46 by sazouaka          #+#    #+#             */
/*   Updated: 2020/10/02 19:33:48 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

int				find_caracter(char *str, char c)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

t_l				*get_var_list_1(t_l **head, char *str)
{
	t_l		*node;
	char	*tmp;

	node = *head;
	while (node->next)
		node = node->next;
	if (!(node->next = (t_l *)malloc(sizeof(t_l))))
		return (NULL);
	node = node->next;
	if (find_caracter(str, '{') == 1)
		tmp = ft_strjoin(str, "}");
	if (find_caracter(str, '{') == 0)
		tmp = ft_strdup(str);
	node->data = ft_strjoin("$", tmp);
	ft_strdel(&tmp);
	node->next = NULL;
	return (*head);
}

t_l				*get_var_list(char *str, t_l *head)
{
	char	*tmp;

	if (!head)
	{
		if (!(head = (t_l *)malloc(sizeof(t_l))))
			return (NULL);
		if (find_caracter(str, '{') == 1)
			tmp = ft_strjoin(str, "}");
		if (find_caracter(str, '{') == 0)
			tmp = ft_strdup(str);
		head->data = ft_strjoin("$", tmp);
		ft_strdel(&tmp);
		head->next = NULL;
		return (head);
	}
	get_var_list_1(&head, str);
	return (head);
}

char			**var_search_2(char *str)
{
	t_variable	*var;
	t_l			*var_list;
	char		**var_tab;

	var = g_var.var;
	var_list = NULL;
	var_tab = NULL;
	while (var)
	{
		if (!ft_strncmp(str, var->key, ft_strlen(str)))
			var_list = get_var_list(var->key, var_list);
		var = var->next;
	}
	var_tab = tab_from_list(var_list);
	free_list(var_list);
	return (var_tab);
}

char			**var_search_1(char *str)
{
	t_variable	*var;
	t_l			*var_list;
	char		**var_tab;
	char		*tmp1;
	char		*tmp2;

	var = g_var.var;
	var_list = NULL;
	var_tab = NULL;
	tmp1 = ft_strsub(str, 1, (int)ft_strlen(str));
	while (var)
	{
		if (!ft_strncmp(tmp1, var->key, ft_strlen(tmp1)))
		{
			tmp2 = ft_strjoin("{", var->key);
			var_list = get_var_list(tmp2, var_list);
		}
		var = var->next;
	}
	ft_strdel(&tmp1);
	var_tab = tab_from_list(var_list);
	free_list(var_list);
	return (var_tab);
}
