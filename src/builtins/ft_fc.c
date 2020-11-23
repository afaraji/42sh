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

void	prit_to_file(t_hist *head)
{
	int		fd;
	t_hist	*node;

	if ((fd = open("/tmp/tmp_file", O_RDONLY | O_CREAT, 0600)) == -1)
	{
		ft_print(STDERR, "couldn't create nor tmp_file.\n");
		return (NULL);
	}
	node = head;
	while (node)
	{
		ft_print(fd, "%s\n", node->hist_str);
		node = node->next;
	}
	close(fd);
}

// void	open_with_editor()
// we were here working on opening file for read.

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

int		fc_error_1(void)
{
	ft_print(STDERR, "42sh: fc: history specification out of range\n");
	return (0);
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

int		flag_tab(char *tabl, char **flag, int *i, char **editor)
{
	int		j;
	int		z;
	int		found_e;

	z = 1;
	found_e = 0;
	*editor = NULL;
	while (flag[*i][z])
	{
		j = flag_search(flag[*i][z]);
		if (j == -1)
		{
			ft_print(STDERR, "42sh: fc: %c:invalid option\n", flag[*i][z]);
			ft_print(STDERR, "fc: usage: fc [-e ename] [-nlr] [first] [last] or fc -s [pat=rep] [cmd]\n");
			return (0);
		}
		else
		{
			tabl[j] = flag[*i][z];
			if (j == 1)
				found_e = 1;
		}
		z++;
	}
	if (found_e)
	{
		if (flag[*i + 1])
		{
			(*i)++;
			*editor = ft_strdup(flag[*i + 1]);
		}
		else
		{
			/*
			**	bash: fc: -e: option requires an argument
			**	fc: usage: fc [-e ename] [-nlr] [first] [last] or fc -s [pat=rep] [cmd]
			*/
		}
	}
	return (1);
}

int		get_flag_tab(char *tabl, char **flag, int *index)
{
	int		i;
	char	*editor;

	i = 0;
	while (i < (FLGSIZE - 1))
		tabl[i++] = '0';
	i = 1;
	while (flag[i])
	{
		if (flag[i][0] == '-' && flag[i][1] == '\0')
			return (fc_error_1());
		if (flag_tab(tabl, flag, &i, &editor) == 0)
			return (0);
		i++;
	}
	*index = i;
	return (1);
}

int		get_first_last(char **flag, int idx, char **first, char **last)
{
	if (flag[idx] && flag[idx + 1] && flag[idx + 2])
	{
		ft_print(STDERR, "fc: too many arguments\n");
		return (0);
	}
	else if (flag[idx] && flag[idx + 1])
	{
		*first = ft_strdup(flag[idx]);
		*last = ft_strdup(flag[idx] + 1);
	}
	else if (flag[idx])
	{
		*first = ft_strdup(flag[idx]);
		*last = ft_strdup(flag[idx]);
	}
	else
	{
		*first = NULL;
		*last = NULL;
	}
	return (1);
}

int		ft_fc(char **flag)//return (0) on SUCCESS else return > 0
{
	t_hist	*head;
	char	table[FLGSIZE];
	char	*first;
	char	*last;
	int		index;

	ttyfd = fopen("/dev/ttys002", "w");
	if (get_flag_tab(table, flag, &index) == 0)
		return (1);
	if (get_first_last(flag, index, &first, &last) == 0)
		return (1);
	
	fprintf(ttyfd, "fst[%s] --- lst[%s]\n", first, last);
	head = g_var.history;
	if (head)
	{
		if (table[4] == 's')
		{
			fprintf(ttyfd, "----->done\n");
			exec_last_cmd();
		}
		else
		{	
			// i should have my list from (first to last)
			if (table[0] == 'l')
			{
				if (table[3] == 'r')
					//reverse_list();
				if (table[2] == 'n')
				{
					//supress_nbrs(); //print_output_without_numbers
				}
				else
				{
					// fc_l(head); // print with numbers
				}
			}
			else
			{
				if (table[3] == 'r')
				{
					//revers_list()
				}
				//print_to_file(list)
				//open_with_editor("/tmp/tmp_file", table[1],editor)
				
			}
			// else if (table[1] == 'e')
			// {
			// 	edit_exec_cmd();
			// }
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
