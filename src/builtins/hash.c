/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 18:27:35 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/03 20:46:32 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/hash_table.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"

static int	hash_print(void)
{
	char	**table;
	int		i;

	i = 0;
	table = hash_table();
	if (!table)
		ft_putendl("hash: hash table empty");
	else
		while (table[i])
			ft_putendl(table[i++]);
	table ? ft_str2del(table) : 0;
	return (0);
}

static int	hash_clear(void)
{
	int		fd;

	if ((fd = get_fd('t')) != -1)
		close(fd);
	return (0);
}

int			built_in(char *cmd)
{
	char	**builtins;
	int		ret;
	int		i;

	i = 0;
	ret = 0;
	builtins = ft_strsplit(BUILTINS, '|');
	while (builtins[i] && !ret)
	{
		if (!ft_strcmp(cmd, builtins[i]))
			ret = 1;
		i++;
	}
	free_tab(builtins);
	return (ret);
}

static int	hash_entries(char **av)
{
	char	*tmp;
	int		ret;
	int		i;

	i = 1;
	ret = 0;
	tmp = NULL;
	while (av[i])
	{
		if (!ft_strchr(av[i], '/') && !built_in(av[i]) && !is_alias(av[i]))
		{
			tmp = hash_chck(av[i]);
			if (!tmp)
			{
				ft_putstr_fd("hash: no such command: ", 2);
				ft_putendl_fd(av[i], 2);
				ret = ret | 1;
			}
			else
				ft_strdel(&tmp);
		}
		i++;
	}
	return (ret);
}

int			ft_hash(char *cmd, char **av)
{
	(void)cmd;
	if (!av[1])
		return (hash_print());
	else
	{
		if (av[1][0] == '-' && ft_strcmp(av[1], "-r"))
		{
			ft_putstr_fd("hash: bad option: ", 2);
			ft_putendl_fd(av[1], 2);
		}
		else if (!ft_strcmp(av[1], "-r"))
		{
			if (av[2])
				ft_putendl_fd("hash: too many arguments", 2);
			else
				return (hash_clear());
		}
		else
			return (hash_entries(av));
	}
	return (1);
}
