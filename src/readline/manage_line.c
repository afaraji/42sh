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
			if (ft_strstr(node->hist_str, term->line->str) != NULL)// haystack, needle: find needl in haystack
			{
				*indice = (node->index > 1) ? node->index : 1;
				return (ft_strdup(node->hist_str));
			}
			node = node->prec;
		}
	}
	return (NULL);
}

void		move_left(int j)
{	
	while (j >= 0)
	{
		tputs(tgetstr("le", NULL), 1, ft_intputchar);
		j--;
	}
}

void		move_right(int i)
{
	while (i >= 0)
	{
		tputs(tgetstr("nd", NULL), 1, ft_intputchar);
		i--;
	}
}

void		delet_line(t_terminal *term, int len)
{
	int	i;

	i = len;
	if (!term)
		return;
	while (i)
	{
		tputs(tgetstr("nd", NULL), len, ft_intputchar);
		i--;
	}
	while (i < len)
	{
		tputs(tgetstr("le", NULL), len, ft_intputchar);
		tputs(tgetstr("dc", NULL), len, ft_intputchar);
		i++;
	}
}

int			ft_putline(t_terminal *term, char *line)
{
	static	int	len;
	int		j;
	int		i;

	i = 0;
	j = 0;
	if (line)
	{
		tputs(tgetstr("up", NULL), 1, ft_intputchar);
		j = 10 + ft_strlen(term->line->str);
		move_left(j);
		delet_line(term, len);
		ft_putstr(line);
		len = ft_strlen(line);
		tputs(tgetstr("do", NULL), 1, ft_intputchar);
		i = 13 + ft_strlen(term->line->str);
		move_right(i);
		ft_strdel(&line);
		return (1);
	}
	return (0);
}

void		print_search(t_terminal *term, t_hist **head, int *indice)
{
	char	*line;

	term->line->str = join_line(term->line->str, term->buff, term->line->curs);
	line = incremental_search(term, head, indice);
	display_line(term->line);
	go_right(term->line);
	ft_putline(term, line);
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
			return (ctrl_c_d(term, mult_line));
		}
		if (term->buff == CTRL_L && mult_line == 0)
		{
			ctrl_l(term->line->str);
		}
		if (ft_isprint(term->buff))
		{
			print_search(term, head, &indice);
			continue;
		}
		if (term->buff == DEL)// !!!
		{
			indice = 0;
		 	del_char(term->line);
			continue;
		}
		if (term->buff == ENTER)
		{
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
			ft_putline(term, line);
		}
		else
		{
			if (line)
				ft_strdel(&line);
			ft_putchar('\n');
			return (ft_strdup(""));
		}
	}
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
			return(ctrl_r(term, his_head, mult_line));
		if (printable(term, his_head, mult_line))
			break ;
		else if (!(ft_isprint(term->buff)))
			unprint_manage(term, his_head, &(g_var.cpy_past));
	}
	tmp = ft_strdup(term->line->str);
	free_term(&term);
	return (tmp);
}
