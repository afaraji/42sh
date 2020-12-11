/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmdargs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 18:19:11 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/10 11:29:16 by awali-al         ###   ########.fr       */
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

	b_in_list = ft_strsplit(BUILTINS, '|');
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

t_l		*var_sub(t_l *head)
{
	t_l		*node;
	t_l		*next_node;
	char	**t;
	int		i;

	next_node = head->next;
	head->data = str_dollar_sub(head->data);
	t = ft_strsplit(head->data, ' ');
	ft_strdel(&(head->data));
	i = 1;
	if (!t || !t[0])
	{
		head->data = ft_strdup("");
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
	node->next = next_node;
	return (head);
}

char	*read_and_join(int fd)
{
	char	*tmp;
	char	*line;
	char	*str;
	char	*tmp2;

	str = ft_strdup("");
	while (get_next_line(fd, &line))
	{
		tmp = ft_strtrim(line);
		ft_strdel(&line);
		tmp2 = ft_4strjoin(str, " ", tmp, "");
		ft_strdel(&tmp);
		ft_strdel(&str);
		str = tmp2;
	}
	close(fd);
	return (str);
}

char	*cmd_substitute2(char *s)
{
	char	*tmp;
	int		fd;
	pid_t	pid;

	tmp = ft_strjoin(s, " > /tmp/cmd_sub");
	system_calls("fork", pid = fork(), -1);
	if (pid == 0)
	{
		fd = open("/tmp/cmd_sub", O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd < 0)
			exit(1);
		dup2(STDOUT, fd);
		main_parse(tmp);
	}
	waitpid(pid, &fd, 0);
	fd = open("/tmp/cmd_sub", O_RDONLY);
	if (fd < 0)
		return (NULL);
	ft_strdel(&tmp);
	return (read_and_join(fd));
}

int		cmd_substitute(t_l *p)
{
	int		i;
	char	*tosub;
	char	*tmp;
	char	*tmp2;


	i = -1;
	tmp = ft_strstr(p->data, "$(");
	// printf("---------1--------\n");
	if (tmp != p->data && *(tmp - 1) == '\\')
		return (0);
	while (tmp[++i])
	{
		if (tmp[i] == ')' && tmp[i - 1] != '\\')
			break ;
	}
	// printf("---------2------[%c]--\n", tmp[i - 1]);
	if (tmp[i] != ')')
		return (1);
	tosub = ft_strsub(tmp, 0, i + 2);
	tmp = cmd_substitute2(tosub);
	tmp2 = p->data;
	p->data = ft_replaceword(p->data, tosub, tmp);
	// should split new p->data using whitespaces
	ft_strdel(&tmp2);
	ft_strdel(&tmp);
	ft_strdel(&tosub);
	return (0);
}

int		cmd_sub(t_l *list)
{
	t_l	*node;

	node = list;
	while (node)
	{
		if (ft_strstr(node->data, "$(") && cmd_substitute(node))
		{
			ft_print(STDERR, "shell: arithmetic error: ')' missing\n");
			return (1);
		}
		node = node->next;
	}
	return (0);
}

int		param_expand(t_l *list)
{
	t_l	*node;

	node = list;
	while (node)
	{
		if (expansions_dispatcher(&(node->data)))
		{
			// error msg ?
			return (1);
		}
		node = node->next;
	}
	return (0);
}

char	**quote_removal(char **av)
{
	int	i;
	int	j;

	i = 0;
	while (av[i])
	{
		j = 0;
		while (av[i][j] && (av[i][j] != '"' || (j > 1 && av[i][j - 1] != '\\')))
			j++;
		if (av[i][j] == '"')
		i++;
	}
	return (av);
}

char	**get_arg_var_sub(t_simple_cmd *cmd)
{
	t_l		*list;
	char	**table;

	list = get_args(cmd);
	if (param_expand(list) || cmd_sub(list))
		return (NULL);
	table = list_to_tab(list);
	// table = dollar_subtutution(table);
	table = expand_pattern(table);
	// table = quote_removal(table);
	return (table);
}

	// tot = fopen("/dev/ttys001", "w");
	// fprintf(tot, "-------------start---------------\n");
	// for (t_l *p = list; p ; p = p->next)
	// 	fprintf(tot, "--->[%s]<---\n", p->data);
	// fprintf(tot, "-----------end-----------------\n");

/*
** Shell Parameter Expansion
** Command Substitution
** Arithmetic Expansion
** Word Splitting
** Filename Expansion
** Quote Removal
*/
