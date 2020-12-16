/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pipe_sec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 17:37:02 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/16 13:43:50 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

char			*get_cmd_from_tokens(t_list_token *start, t_list_token *end)
{
	t_list_token	*node;
	char			*str;
	char			*tmp;
	char			*token;

	node = start;
	str = ft_strdup("");
	while (node && node != end)
	{
		if (node->type == WORD)
			token = node->data;
		else
			token = tokentoa(node->type);
		tmp = ft_strjoin(str, token);
		ft_strdel(&str);
		str = tmp;
		node = node->next;
	}
	return (str);
}

t_simple_cmd	*malloc_simple_cmd(t_list_token *start, t_list_token *end)
{
	t_simple_cmd	*ret;

	ret = (t_simple_cmd *)malloc(sizeof(t_simple_cmd));
	if (!ret)
		return (NULL);
	ret->name = NULL;
	ret->word = NULL;
	ret->suffix = NULL;
	ret->prefix = NULL;
	ret->command = get_cmd_from_tokens(start, end);
	return (ret);
}

t_simple_cmd	*get_simple_cmd(t_list_token *start, t_list_token *end)
{
	t_simple_cmd *ret;

	if (g_var.errno)
		return (NULL);
	ret = malloc_simple_cmd(start, end);
	ret->prefix = cmd_prefix(&start, &end);
	if (!g_var.errno && ret->prefix)
	{
		ret->word = cmd_word(&start, &end);
		if (ret->word)
		{
			ret->suffix = cmd_suffix(&start, &end);
			return (ret);
		}
		return (ret);
	}
	else if (!g_var.errno && (ret->name = cmd_name(&start, &end)))
	{
		ret->suffix = cmd_suffix(&start, &end);
		return (ret);
	}
	ft_print(STDERR, "shell: syntax error, unexpected token.\n");
	g_var.errno = 122;
	free_simple_cmd(ret);
	return (NULL);
}

t_pipe_seq		*ast_fill(t_list_token *tokens, t_list_token *node, int right)
{
	t_pipe_seq		*tmp;

	if (g_var.errno)
		return (NULL);
	tmp = (t_pipe_seq *)malloc(sizeof(t_pipe_seq));
	if (!tmp || g_var.errno)
		return (NULL);
	tmp->left = get_simple_cmd(tokens, node);
	if (right && !g_var.errno)
		tmp->right = ast(node->next);
	else
		tmp->right = NULL;
	if (g_var.errno)
	{
		free_ast(tmp);
		return (NULL);
	}
	return (tmp);
}

t_pipe_seq		*ast(t_list_token *tokens)
{
	t_list_token	*node;
	t_list_token	*prec;

	if (!tokens || g_var.errno)
	{
		if (!g_var.errno)
			g_var.errno = 122;
		ft_print(STDERR, "syntax error, unexpected token.\n");
		return (NULL);
	}
	node = tokens;
	while (node)
	{
		if (node->type == PIP)
			return (ast_fill(tokens, node, 1));
		prec = node;
		node = node->next;
	}
	return (ast_fill(tokens, prec, 0));
}
