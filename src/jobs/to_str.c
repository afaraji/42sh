/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_str.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 18:15:32 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/15 20:00:48 by awali-al         ###   ########.fr       */
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

char	*io_redirect_to_str(t_io_redirect *io)
{
	char	*typ;
	char	*num;
	char	*str;

	typ = tokentoa(io->redirect_type);
	num = ft_itoa(io->io_num);
	str = ft_4strjoin(num, typ, io->filename, "");
	ft_strdel(&num);
	return (str);
}

char	*args_to_str(t_cmd_suffix *suff, t_cmd_prefix *pref)
{
	char	*str;
	char	*tmp;
	char	*tmp2;

	str = ft_strdup(" ");
	while (pref)
	{
		if (pref->io_redirect)
			tmp2 = io_redirect_to_str(pref->io_redirect);
		else if (pref->ass_word)
			tmp2 = ft_4strjoin(pref->ass_word->key, "=",
													pref->ass_word->value, "");
		tmp = ft_4strjoin(str, " ", tmp2, " ");
		ft_strdel(&str);
		ft_strdel(&tmp2);
		str = tmp;
		pref = pref->prefix;
	}
	return (args_to_str2(suff, str));
}

char	*simple_cmd_to_str(t_simple_cmd *cmd)
{
	char	*name;
	char	*str;
	char	*args;

	if (cmd->name || cmd->word)
		name = (cmd->name) ? cmd->name : cmd->word;
	else
		name = "";
	args = args_to_str(cmd->suffix, cmd->prefix);
	str = ft_strjoin(name, args);
	ft_strdel(&args);
	return (str);
}

char	*ast_to_str(t_pipe_seq *cmd)
{
	char	*tmp;
	char	*args;
	char	*name;
	char	*str;
	char	*pipe;

	str = ft_strdup("");
	while (cmd)
	{
		name = (cmd->left->name) ? cmd->left->name : cmd->left->word;
		name = (name) ? name : "";
		args = args_to_str(cmd->left->suffix, cmd->left->prefix);
		pipe = (cmd->right) ? " | " : "";
		tmp = ft_4strjoin(str, name, args, pipe);
		ft_strdel(&str);
		ft_strdel(&args);
		str = tmp;
		cmd = cmd->right;
	}
	return (str);
}

char	*and_or_to_str(t_and_or *cmd)
{
	char	*ast;
	char	*token;
	char	*str;
	char	*tmp;

	str = ft_strdup("");
	while (cmd)
	{
		ast = ast_to_str(cmd->ast);
		if (cmd->dependent)
			token = (cmd->dependent == 1) ? " && " : " || ";
		else
			token = "";
		tmp = ft_4strjoin(str, token, ast, "");
		ft_strdel(&str);
		ft_strdel(&ast);
		str = tmp;
		cmd = cmd->next;
	}
	return (str);
}
