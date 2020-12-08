/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_nofork.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 18:13:35 by afaraji           #+#    #+#             */
/*   Updated: 2020/11/03 18:14:02 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

t_variable	*var_list_dup(t_variable *src)
{
	t_variable	*node;
	t_variable	*tmp;
	t_variable	*head;

	head = var_dup(src);
	node = src;
	tmp = head;
	while (node->next)
	{
		tmp->next = var_dup(node->next);
		tmp = tmp->next;
		node = node->next;
	}
	return (head);
}

void		reset_in_out(int set)
{
	static int in = -1;
	static int out = -1;
	static int err = -1;

	if (set)
	{
		in = dup2(STDIN, 250);
		out = dup2(STDOUT, 251);
		err = dup2(STDOUT, 252);
	}
	else if (in != -1 && out != -1 && err != -1)
	{
		dup2(in, STDIN);
		if (in != STDIN)
			close(in);
		dup2(out, STDOUT);
		if (in != STDOUT)
			close(out);
		dup2(err, STDERR);
		if (in != STDERR)
			close(err);
	}
}

void		close_open_fd(t_cmd_prefix *pref, t_cmd_suffix *suff)
{
	t_cmd_prefix	*p;
	t_cmd_suffix	*s;
	int				io;

	p = pref;
	s = suff;
	while (p)
	{
		if (p->io_redirect && (io = p->io_redirect->io_num) > 2)
		{
			close(io);
		}
		p = p->prefix;
	}
	while (s)
	{
		if (s->io_redirect && (io = s->io_redirect->io_num) > 2)
		{
			close(io);
		}
		s = s->suffix;
	}
}

int			exec_no_fork_builtin(t_simple_cmd *cmd, char **av)
{
	int			status;
	t_variable	*tmp;
	char		**env;

	tmp = var_list_dup(g_var.var);
	reset_in_out(SETDFL);
	if (do_prefix(cmd->prefix, tmp, 0) || do_suffix(cmd->suffix))
	{
		free_t_var(tmp);
		reset_in_out(GETDFL);
		return (1);
	}
	env = env_to_tab(tmp, 0);
	free_t_var(tmp);
	status = builtins(av[0], av, env);
	reset_in_out(GETDFL);
	close_open_fd(cmd->prefix, cmd->suffix);
	free_tab(env);
	return (status);
}

int			exec_no_fork(t_pipe_seq *cmd, int bg)
{
	char		**av;
	int			status;

	if (cmd->right == NULL && !bg)
	{
		av = get_arg_var_sub(cmd->left);
		if (av && is_builtin(av[0]))
		{
			status = exec_no_fork_builtin(cmd->left, av);
			free_tab(av);
			return (status);
		}
		free_tab(av);
		if (!(cmd->left->name) && !(cmd->left->word))
		{
			reset_in_out(SETDFL);
			status = do_prefix(cmd->left->prefix, g_var.var, 1);
			reset_in_out(GETDFL);
			return (status);
		}
	}
	return (-42);
}
