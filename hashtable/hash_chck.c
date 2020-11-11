/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_chck.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:39:55 by awali-al          #+#    #+#             */
/*   Updated: 2020/11/11 19:12:11 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash_table.h"

static char	*table_check(char *cmd, char **table)
{
	char			*tmp;
	int				i;

	if (table)
	{
		tmp = ft_strappend(cmd, '=');
		i = 0;
		while (table[i] && table[i] != ft_strstr(table[i], tmp))
			i++;
		ft_strdel(&tmp);
		if (table[i])
		{
			tmp = ft_strdup(table[i] + ft_strlen(cmd) + 1);
			return (tmp);
		}
	}
	return (NULL);
}

static char	*path_check(char *cmd)
{
	struct dirent	*file;
	char			**table;
	char			*ret;
	DIR				*fd;
	int				i;

	ret = NULL;
	table = ft_strsplit(getenv("PATH"), ':');
	i = 0;
	if (table)
		while (table[i] && !ret)
		{
			fd = opendir(table[i]);
			while ((file = readdir(fd)) && !ret)
			{
				if (!ft_strcmp(file->d_name, cmd))
					ret = ft_strstich(table[i], '/', cmd);
			}
			i++;
			closedir(fd);
		}
	table ? ft_str2del(table) : 0;
	return (ret);
}

char		*hash_chck(char *cmd)
{
	char			**table;
	char			*ret;

	table = hash_table();
	ret = table_check(cmd, table);
	if (!ret)
	{
		ret = path_check(cmd);
		if (ret)
			hash_add(cmd, ret);
	}
	table ? ft_str2del(table) : 0;
	return (ret);
}
