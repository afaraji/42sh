/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/18 19:33:46 by sazouaka          #+#    #+#             */
/*   Updated: 2020/01/18 19:33:48 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

FILE *ttyfd;

t_terminal	*init_term(char *prmt)
{
	int			tmp;
	t_terminal	*term;

	if (read(0, &tmp, 0) < 0)
		return (NULL);
	term = initiate_unprint_var();
	term->line = init_line(prmt);
	ft_prompt(prmt);
	return (term);
}

char		*ctrl_c_d(t_terminal *term, int mult_line)
{
	if (term->buff == CTRL_C)
	{
		free_term(&term);
		ft_putstr_fd("^C\n", 1);
		tputs(tgetstr("cd", NULL), 1, ft_intputchar);
		if (mult_line != 0)
		{
			return (ft_strdup("\033"));
		}
		return (ft_strdup(""));
	}
	else
	{
		free_term(&term);
		ft_putchar('\n');
		if (mult_line == 0)
			return (ft_strdup("exit"));
		return (ft_strdup("\030"));
	}
}

void		ctrl_l(char *str)
{
	ft_putstr_fd("\033[H\033[2J", 1);
	ft_prompt("$> ");
	ft_putstr(str);
}

void		unprint_manage(t_terminal *term, t_hist **his_head, char **to_past)
{
	int		unprint_ret;

	unprint_ret = unprintable(term, his_head, to_past);
	if (unprint_ret == 2)
	{
		ft_prompt("\n$> ");
		ft_putstr(term->line->str);
	}
}

char		*incremental_search(t_terminal *term, t_hist **head, int *indice)
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

void		check_first_entry(t_terminal *term, t_hist **head)
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

void		check_second_entry(t_terminal *term, char *s)
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

void		display_result(t_terminal *term, char *s, t_hist **head)
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
	(*head)->s_chr = ft_strdup(term->line->str);
	ft_strdel(&(term->line->str));
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

void		ft_put_line(char *line, t_terminal *term, t_hist **head)
{
	char	*tmp;
	char	*tmp1;

	if (line)
	{
		tmp = ft_strdup(term->line->str);
		ft_strdel(&(term->line->str));
		tmp1 = ft_strjoin(line, "\n");
		term->line->str = ft_strjoin(tmp1, "bck-i-search: ");
		display_result(term, tmp, head);
	}
	else
	{
		display_line(term->line);
		go_right(term->line);
	}
}

char		*print_search(t_terminal *term, t_hist **head, int *indice)
{
	char	*line;

	term->line->str = join_line(term->line->str, term->buff, term->line->curs);
	line = incremental_search(term, head, indice);
	ft_put_line(line, term, head);
	return (line);
}

char		*bck_i_search(t_terminal *term, t_hist **head, int mult_line)
{
	char	*line;
	int		indice;

	indice = 0;
	line = NULL;
	while (1)
	{
		term->buff = 0;
		read(0, &term->buff, 4);
		if (term->buff == CTRL_C || (term->buff == CTRL_D &&
											!ft_strcmp(term->line->str, "")))
		{
			(*head)->s_chr = NULL;
			return (ctrl_c_d(term, mult_line));
		}
		if (term->buff == CTRL_L && mult_line == 0)
		{
			ctrl_l(term->line->str);
		}
		if (ft_isprint(term->buff))
		{
			line = print_search(term, head, &indice);
			continue;
		}
		if (term->buff == DEL)
		{
			indice = 0;
			del_char(term->line);
			continue;
		}
		if (term->buff == ENTER)
		{
			(*head)->s_chr = NULL;
			ft_putchar('\n');
			if (line)
				return (line);
			else
				return (ft_strdup(""));
		}
		if (term->buff == CTRL_R)
		{
			indice--;
			line = incremental_search(term, head, &indice);
			ft_put_line(line, term, head);
		}
		else
		{
			(*head)->s_chr = NULL;
			ft_putchar('\n');
			return (ft_strdup(""));
		}
	}
	(*head)->s_chr = NULL;
	return (ft_strdup(""));
}

char		*ctrl_r(t_terminal *term, t_hist **head, int mult_line)
{
	if (!head || !term)
		return (NULL);
	tputs(tgetstr("do", NULL), 1, ft_intputchar);
	ft_putstr("bck-i-search: ");
	return (bck_i_search(term, head, mult_line));
}

char		*manage_line(char *prompt, t_hist **his_head, int mult_line)
{
	t_terminal	*term;
	char		*tmp;

	ttyfd = fopen("/dev/ttys002", "w");
	if (!(term = init_term(prompt)))
		return (NULL);
	while (1)
	{
		term->buff = 0;
		read(0, &term->buff, 4);
		if (term->buff == CTRL_C || (term->buff == CTRL_D &&
											!ft_strcmp(term->line->str, "")))
			return (ctrl_c_d(term, mult_line));
		if (term->buff == CTRL_L && mult_line == 0)
			ctrl_l(term->line->str);
		if (term->buff == CTRL_R)
			return (ctrl_r(term, his_head, mult_line));
		if (printable(term, his_head, mult_line))
			break ;
		else if (!(ft_isprint(term->buff)))
			unprint_manage(term, his_head, &(g_var.cpy_past));
	}
	tmp = ft_strdup(term->line->str);
	free_term(&term);
	return (tmp);
}
