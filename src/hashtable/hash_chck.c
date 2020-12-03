/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_chck.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 02:39:55 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/03 02:33:49 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/hash_table.h"

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
	table = NULL;
	table = ft_strsplit(getenv("PATH"), ':');
	i = 0;
	if (table)
		while (table[i] && !ret)
		{
			fd = opendir(table[i]);
			while ((file = readdir(fd)) && !ret)
			{
				if (file && !ft_strcmp(file->d_name, cmd))
					ret = ft_strstich(table[i], '/', cmd);
			}
			closedir(fd);
			i++;
		}
	table ? ft_str2del(table) : 0;
	return (ret);
}

char		*hash_chck(char *cmd)
{
	char			**table;
	char			*ret;

	table = hash_table();
	ret = NULL;
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
