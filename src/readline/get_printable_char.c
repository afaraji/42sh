/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_printable_char.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <sazouaka@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 00:03:32 by sazouaka          #+#    #+#             */
/*   Updated: 2020/03/04 00:03:34 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

int		get_cmd(t_terminal *term, t_hist **his_head, int mult_line)
{
	char	*tmp;
	term->line->str = join_line(term->line->str, term->buff, term->line->curs);
	display_line(term->line);
	go_right(term->line);
	if (term->buff == ENTER)
	{
		tmp = ft_strtrim(term->line->str);
		if (ft_strcmp(tmp, "") != 0	|| (mult_line != 0 && mult_line != -1))
			add_cmd_to_his_list(term->line->str, his_head, mult_line);
		ft_strdel(&tmp);
	}
	return (0);
}

void	printable_2(t_terminal *term)
{
	term->select->on = 0;
	tputs(tgetstr("sc", NULL), 1, ft_intputchar);
	display_line_from_begin(term->line);
	tputs(tgetstr("rc", NULL), 1, ft_intputchar);
}

int		is_all_print(char *str, int size)
{
	int		i;

	i = 0;
	while (i < size && str[i])
	{
		if (!ft_isprint(str[i]) && str[i] != '\n' && str[i] != '\t')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int		printable_1(t_terminal *term, t_hist **his_head, int mult_line)
{
	int	curs;

	term->tab_on = 0;
	if (term->select->on == 1)
		printable_2(term);
	else
	{
		get_cmd(term, his_head, mult_line);
		if (term->buff == ENTER)
		{
			curs = term->line->curs;
			while (curs <= (int)ft_strlen(term->line->str))
			{
				go_right(term->line);
				curs++;
			}
			ft_putchar('\n');
			return (1);
		}
	}
	return (0);
}

int		printable(t_terminal *term, t_hist **his_head, int mult_line)
{
	int		i;
	char	buff[4];
	int		tmp;

	i = 0;
	ft_memcpy(buff, &(term->buff), 4);
	tmp = term->buff;
	if (!is_all_print(buff, 4))
		return (0);
	while (i < 4)
	{
		term->buff = buff[i];
		if ((ft_isprint(term->buff) || term->buff == ENTER))
		{
			if (printable_1(term, his_head, mult_line) == 1)
				return (1);
		}
		i++;
	}
	term->buff = tmp;
	return (0);
}
