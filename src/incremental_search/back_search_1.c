/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   back_search_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <sazouaka@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:49:25 by sazouaka          #+#    #+#             */
/*   Updated: 2020/11/20 16:49:26 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

char	*print_search(t_terminal *term, t_hist **head, int *indice)
{
	char	*line;

	term->line->str = join_line(term->line->str, term->buff, term->line->curs);
	line = incremental_search(term, head, indice);
	ft_put_line(line, term, head);
	return (line);
}

char	*bck_search_2(t_terminal *term, t_hist **head, char *line, int mlt_line)
{
	if (term->buff == CTRL_C || (term->buff == CTRL_D &&
										!ft_strcmp(term->line->str, "")))
	{
		if (line)
			ft_strdel(&line);
		return (ctrl_c_d(term, mlt_line));
	}
	else if (term->buff == ENTER)
	{
		(*head)->s_chr = NULL;
		ft_putchar('\n');
		if (line)
			return (line);
		else
			return (ft_strdup(""));
	}
	else
	{
		if (line)
			ft_strdel(&line);
		(*head)->s_chr = NULL;
		ft_putchar('\n');
		return (ft_strdup(""));
	}
}

void	back_search_1(t_terminal *term, t_hist **head, char **line, int *indice)
{
	(*indice)--;
	if (*line)
		ft_strdel(line);
	*line = incremental_search(term, head, indice);
	ft_put_line(*line, term, head);
}

char	*bck_search(t_terminal *term, t_hist **head, int mult_line)
{
	char	*line;
	int		indice;

	indice = 0;
	line = NULL;
	while (1)
	{
		term->buff = 0;
		read(0, &term->buff, 4);
		if (ft_isprint(term->buff))
		{
			if (line)
				ft_strdel(&line);
			line = print_search(term, head, &indice);
		}
		else if (term->buff == DEL)
		{
			indice = 0;
			del_char(term->line);
		}
		else if (term->buff == CTRL_R)
			back_search_1(term, head, &line, &indice);
		else
			return (bck_search_2(term, head, line, mult_line));
	}
	(*head)->s_chr = NULL;
	return (ft_strdup(""));
}

char	*ctrl_r(t_terminal *term, t_hist **head, int mult_line)
{
	char	*tmp;

	if (!head || !term)
		return (NULL);
	tputs(tgetstr("do", NULL), 1, ft_intputchar);
	ft_putstr("bck-i-search: ");
	tmp = bck_search(term, head, mult_line);
	if (term)
	{
		printf("^^&111** \n");
		free_term(&term);
		printf("^^&222** \n");
	}
	return (tmp);
}
