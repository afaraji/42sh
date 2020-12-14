/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 22:35:31 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/10 11:34:42 by awali-al         ###   ########.fr       */
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
#include "../../trash/debug_prints.c"//remove this

int		is_valid_word(char *s)
{
	int	i;

	if (s[0] != '_' && !ft_isalpha(s[0]))
		return (0);
	i = 1;
	while (s[i])
	{
		if (s[i] != '_' && !ft_isalnum(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int		is_valid_file(char *file, t_list_token *node)
{
	if (!node || !file)
		return (1);
	if (node->type <= -20 && node->type >= -31)
	{
		if (is_all_digits(file))
		{
			return (0);
		}
	}
	return (1);
}

int		manage_cmd_list(t_cmdlist *cmdlist)
{
	t_cmdlist		*node;
	int				ret;

	node = cmdlist;
	ret = 0;
	while (node)
	{
		// ret = execute(node->and_or, node->bg);
		ret = job_control(node->and_or, node->bg);
		node = node->next;
	}
	free_cmd_list(cmdlist);
	return (ret);
}

int		join_escape(t_list_token *token)
{
	t_list_token	*node;

	node = token;
	while (node)
	{
		if (node->type == ESCAPE)
		{
			node->type = WORD;
			node->data[0] = '\0';
		}
		if (node->type == DQUOTE && node->is_ok)//recently added [need to be verified in other situations]
		{
			node->data = delete_escape(node->data);
		}
		node = node->next;
	}
	return (0);
}

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

int		history_sub(t_list_token *tokens)
{
	t_list_token	*node;
	char			*new;
	char			*old;
	char			*tmp;
	t_list_token	*www;
	t_list_token	*zzz;

	node = tokens;
	while (node)
	{
		if ((node->type == WORD || node->type == DQUOTE) && (old = get_event_disignator(node->data)))
		{
			free(fc_history_remove());
			new = history_search(old + 1, &g_var.history);
			if (new == NULL)
			{
				ft_print(STDERR, "shell: %s: event not found.\n", old);
				ft_strdel(&old);
				return (1);
			}
			tmp = ft_replaceword(node->data, old, new);
			ft_strdel(&old);
			ft_strdel(&new);
			www = ft_tokenize(tmp);
			ft_strdel(&tmp);
			zzz = node->next;
			replace_node(&node, &www);
			node = tokens;
			while (node->next)
				node = node->next;
			node->next = zzz;
			tmp = tokens_to_str(tokens);
			fc_history_add(tmp, 1);
			ft_strdel(&tmp);
		}
		node = node->next;
	}
	return (0);
}

int		main_parse(char *line)
{
	t_list_token	*tokens;
	t_cmdlist		*cmdlist;
// debug();
	tokens = ft_tokenize(line);
	ft_strdel(&line);
// fprintf(ttyfd, "--1-->"); token_print(tokens);
	if (lexer(&tokens) || verify_tokens(tokens) || need_append(tokens) ||
															history_sub(tokens))
	{
		free_tokens(tokens);
		return (100);
	}
// 	if (lexer(&tokens))
// 		return (100);
// // fprintf(ttyfd, "--2-->"); token_print(tokens);
// 	if (verify_tokens(tokens))
// 		return (100);
// // fprintf(ttyfd, "--3-->"); token_print(tokens);
// 	if (need_append(tokens))
// 		return (100);
// // fprintf(ttyfd, "--4-->"); token_print(tokens);
// 	if (history_sub(tokens))
// 		return (100);
// fprintf(ttyfd, "--5-->"); token_print(tokens);

	join_escape(tokens);
	join_words(tokens);
	join_words(tokens);
// token_print(tokens);
	// if (expansions(tokens))
	// {
	// 	free_tokens(tokens);
	// 	return (1);
	// }
	here_doc(tokens);
	cmdlist = token_split_sep_op(tokens);
	free_tokens(tokens);
	if (!cmdlist || g_var.errno)
	{
		if (cmdlist)
			free_cmd_list(cmdlist);
		return (42);
	}
	return (manage_cmd_list(cmdlist));
}
