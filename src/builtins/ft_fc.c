/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <sazouaka@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 18:18:50 by sazouaka          #+#    #+#             */
/*   Updated: 2020/11/20 18:18:52 by sazouaka         ###   ########.fr       */
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
void	open_vim()
{
	
}

void	fc_l_print_1(t_hist *head)
{
	t_hist	*node;

	node = head;
	while (node)
	{
		ft_putnbr(node->index);
		ft_putchar('\t');
		ft_putstr(node->hist_str);
		ft_putchar('\n');
		node = node->next;
	}
}

void	fc_l_print_2(t_hist *head, int idx)
{
	t_hist	*node;

	node = head;
	while (node->index < idx)
		node = node->next;
	while (node)
	{
		ft_putnbr(node->index);
		ft_putchar('\t');
		ft_putstr(node->hist_str);
		ft_putchar('\n');
		node = node->next;
	}
}

int		fc_l(t_hist *head)
{
	t_hist	*node;
	int		idx;

	node = head;
	while (node->next)
		node = node->next;
	if (node->index <= 16)
	{
		fc_l_print_1(head);
		return (0);
	}
	else
	{
		idx = node->index - 16 + 1;
		fc_l_print_2(head, idx);
		return (0);
	}
	return (1);
}

int		ft_fc(char **flag)//return (0) on SUCCESS else return > 0
{
	t_hist *head;

	head = g_var.history;
	ttyfd = fopen("/dev/ttys004", "w");
	if (head)
	{
		// if (flag[1] == NULL)
		// {
		// 	open_vim();//to edit the last commande in history
		// }
		if ((ft_strequ(flag[1], "-l")))
		{
			return (fc_l(head));
		}
		// else if ((ft_strequ(flag[1], "-r")))
		// {
		// 	fc_r();
		// }
		// else if ((ft_strequ(flag[1], "-n")))
		// {
		// 	fc_n();
		// }
		// else if ((ft_strequ(flag[1], "-e")))
		// {
		// 	fc_e();
		// }
		// else if ((ft_strequ(flag[1], "-s")))
		// {
		// 	fc_s();
		// }
	}
	return (1);
}
