/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 16:45:21 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/09 20:34:53 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

int		ft_editor(char *editor, char *file)
{
	char	*line;
	int		ret;
	int		fd;
	char	*tmp;

	if (!editor)
		editor = ft_strdup("/usr/bin/vim ");
	else
	{
		tmp = ft_strdup(editor);
		ft_strdel(&editor);
		editor = ft_strjoin(tmp, " ");
		ft_strdel(&tmp);
	}
	line = ft_strjoin(editor, file);
	if (!(ret = main_parse(line)))
	{
		system_calls("open", (fd = open("/tmp/fc_tmp_file", O_RDONLY)), -1);
		while (get_next_line(fd, &line))
			ret = main_parse(line);
		close(fd);
		unlink("/tmp/fc_tmp_file");
	}
	editor ? ft_strdel(&editor) : 0;
	return (ret);
}

int		fc_do_s(char **av, int i)
{
	char	*new;
	char	*old;
	int		first;
	int		index;
	char	*tmp;

	old = NULL;
	new = NULL;
	if (av[i] && (index = is_assword(av[i])))
	{
		old = ft_strsub(av[i], 0, index - 1);
		new = ft_strdup(&av[i][index]);
		i++;
	}
	first = get_index_hist_first(av[i], 0);
	if (!first)
		return (-1);
	tmp = get_hist_node(first)->hist_str;
	tmp = ft_replaceword(tmp, old, new);
	return (main_parse(tmp));
}

int		fc_add_to_file(t_hist *list, char *editor, int r)
{
	t_hist	*node;
	int		fd;

	if ((fd = open("/tmp/fc_tmp_file", O_WRONLY | O_CREAT, 0744)) == -1)
	{
		if (editor)
			ft_strdel(&editor);
		ft_print(STDERR, "couldn't create fc_tmp_file.\n");
		return (-1);
	}
	node = list;
	while (r && node->next)
		node = node->next;
	while (node)
	{
		ft_print(fd, "%s\n", node->hist_str);
		node = (r) ? node->prec : node->next;
	}
	close(fd);
	return (ft_editor(editor, "/tmp/fc_tmp_file"));
}
