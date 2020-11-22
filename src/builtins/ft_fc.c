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

// void	open_vim(t_hist *head)
// {
// 	int		fd;

// 	if ((fd = open("/tmp/tmp_file", O_RDONLY | O_CREAT, 0600)) == -1)
// 	{
// 		ft_print(STDERR, "couldn't create nor tmp_file.\n");
// 		return (NULL);
// 	}
// }

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

int		flag_nbr(char **flag)
{
	int	i;

	i = 0;
	while (flag[i])
		i++;
	return (i);
}
char		*fc_error_1(void)
{
	ft_print(STDERR, "42sh: fc: history specification out of range\n");
	return (NULL);
}

// char		*fc_error_2(void)
// {
// 	ft_print(STDERR, "42sh: fc: invalid option.\n");
// 	ft_print(STDERR, "fc: usage: fc [-e ename] [-nlr] [first] [last] or fc -s [pat=rep] [cmd]\n");
// 	return (NULL);
// }
int		flag_search(char c)
{
	int	i;

	i = 0;
	while (FLAGS[i] != '\0')
	{
		if (FLAGS[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

char	*flag_tab(char *tabl, char **flag, int i)
{
	int		j;
	int		z;

	z = 1;
	while (flag[i][z])
	{
		j = flag_search(flag[i][z]);
		if (j == -1)
		{
			ft_print(STDERR, "42sh: fc: invalid option\n");
		//	write(2, &flag[i][z], 1);
			ft_print(STDERR, "fc: usage: fc [-e ename] [-nlr] [first] [last] or fc -s [pat=rep] [cmd]\n");
			return (NULL);
		}
		else
		{
			tabl[j] = flag[i][z];
			// here I should make rules for the flags that should be ignored
		}
		z++;
	}
	return (tabl);
}

char	*get_flag_tab(int flag_nbr, char **flag)
{
	int		i;
	char	*tabl;

	tabl = (char *)malloc(sizeof(char) * ft_strlen(FLAGS));
	i = 0;
	while (i < (int)ft_strlen(FLAGS))
		tabl[i++] = '0';
	i = 1;
	while (i < flag_nbr && flag[i][0] == '-' && flag[i][1] != '\0')
	{
		if (flag[i][0] == '-')
		{
			if (flag[i][1] == '\0')
				return (fc_error_1());
			// else
			// 	return (fc_error_2());
		}
		if (!(flag_tab(tabl, flag, i)))
			return (NULL);
		i++;
	}
	return (tabl);
}

int		ft_fc(char **flag)//return (0) on SUCCESS else return > 0
{
	t_hist *head;
	char	*table;

	if (!(table = get_flag_tab(flag_nbr(flag), flag)))
		return (1);
	head = g_var.history;
	ttyfd = fopen("/dev/ttys004", "w");
	if (head)
	{
		if (flag[1] == NULL)
		{
			fprintf(ttyfd, "[%s]\n", flag[1]);
			//open_vim();//to edit the last commande in history
		}
		else if ((ft_strequ(flag[1], "-l")))
		{
			return (fc_l(head));
		}
		// else if ((ft_strequ(flag[1], "-r")))
		// {
		// 	reverse_output();
		// }
		// else if ((ft_strequ(flag[1], "-n")))
		// {
		// 	supress_nbrs();
		// }
		// else if ((ft_strequ(flag[1], "-e")))
		// {
		// 	set_editor();
		// }
		// else if ((ft_strequ(flag[1], "-s")))
		// {
		// 	exec_last_cmd();
		// }
	}
	return (1);
}
