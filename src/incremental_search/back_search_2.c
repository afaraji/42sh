/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   back_search_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:49:31 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/09 20:55:31 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

char	*incremental_search(t_terminal *term, t_hist **head, int *indice)
{
	t_hist	*node;

	node = *head;
	if (node)
	{
		while (node->next && node->index != *indice)
			node = node->next;
		while (node)
		{
			if (ft_strstr(node->hist_str, term->line->str) != NULL)
			{
				*indice = (node->index > 1) ? node->index : 1;
				return (ft_strdup(node->hist_str));
			}
			node = node->prec;
		}
	}
	return (NULL);
}

void	check_first_entry(t_terminal *term, t_hist **head)
{
	int		i;
	char	*tmp1;

	i = ft_strlen((*head)->s_chr) - term->line->pmt_s;
	term->line->curs = i;
	tmp1 = ft_strdup(term->line->str);
	ft_strdel(&(term->line->str));
	term->line->str = ft_strdup((*head)->s_chr);
	while (i > 0)
	{
		go_left(term->line);
		i--;
	}
	ft_strdel(&(term->line->str));
	term->line->str = ft_strdup(tmp1);
	ft_strdel(&tmp1);
}

void	check_second_entry(t_terminal *term, char *s)
{
	int	i;

	i = ft_strlen(s) + ft_strlen("bck-i-search: ") - term->line->pmt_s;
	tputs(tgetstr("up", NULL), 1, ft_intputchar);
	while (i > 0)
	{
		tputs(tgetstr("le", NULL), 1, ft_intputchar);
		i--;
	}
}

void	display_result(t_terminal *term, char *s, t_hist **head)
{
	int	i;

	if ((*head)->s_chr)
		check_first_entry(term, head);
	else
		check_second_entry(term, s);
	display_line(term->line);
	i = ft_strlen(term->line->str);
	while (i)
	{
		go_right(term->line);
		i--;
	}
	*head && (*head)->s_chr ? ft_strdel(&((*head)->s_chr)) : 0;
	(*head)->s_chr = ft_strdup(term->line->str);
	term->line->str ? ft_strdel(&(term->line->str)) : 0;
	term->line->str = ft_strdup(s);
	term->line->curs = 0;
	display_line(term->line);
	i = ft_strlen(term->line->str);
	while (i)
	{
		go_right(term->line);
		i--;
	}
}

void	ft_put_line(char *line, t_terminal *term, t_hist **head)
{
	char	*tmp;
	char	*tmp1;

	if (line)
	{
		tmp = ft_strdup(term->line->str);
		if (term->line->str)
			ft_strdel(&(term->line->str));
		tmp1 = ft_strjoin(line, "\n");
		term->line->str = ft_strjoin(tmp1, "bck-i-search: ");
		ft_strdel(&tmp1);
		display_result(term, tmp, head);
		ft_strdel(&tmp);
	}
	else
	{
		display_line(term->line);
		go_right(term->line);
	}
}
