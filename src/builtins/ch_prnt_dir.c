/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ch_prnt_dir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 02:09:44 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/05 12:52:32 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../inc/expansion.h"
#include <stdio.h>

static char	*up_a_parent(char **ret, char *tmp)
{
	char	*buf;
	int		n;
	
	if (ft_strcmp(*ret, "/"))
	{
		buf = *ret;
		if ((n = ft_strrchr(buf, '/') - buf))
			*ret = ft_strsub(buf, 0, n);
		else
			*ret = ft_strsub(buf, 0, 1);
		ft_strdel(&buf);
	}
	return (tmp + 2);
}

static char	*first_str(char *str1, char *str2)
{
	if ((str1 && !str2) || (str1 && str2 && str1 < str2))
		return (str1);
	else if ((!str1 && str2) || (str1 && str2 && str1 > str2))
		return (str2);
	else
		return (NULL);
}

static char	*new_path(char **ret, char *path)
{
	char	*chosen;
	char	*tmp;
	char	*buf;
	int		n;

	chosen = first_str(ft_strstr(path, "/./"), ft_strstr(path, "/../"));
	buf = !ft_strcmp(*ret, "/") ? ft_strdup(*ret) : ft_strjoin(*ret, "/");
	ft_strdel(ret);
	if (chosen)
	{
		n = chosen - path;
		tmp = ft_strsub(path, 0, n);
		*ret = ft_strjoin(buf, tmp);
		ft_strdel(&tmp);
		n = chosen[1] == '/' ? n + 2 : n + 3;
	}
	else
	{
		*ret = ft_strjoin(buf, path);
		n = ft_strlen(path) - 1;
	}
	ft_strdel(&buf);
	return (path + n);
}

// this function needs to be put in a separate file

static int	same_char(char *str, char c)
{
	int		i;

	i = 0;
	while (str[i])
		if (str[i++] != c)
			return (1);
	return (0);
}

static char	*parent_dots(char *path, char *pwd)
{
	char	*ret;
	int		i;

	// ret = ft_strdup(var_get_value("PATH", 1)); replaces the following line and you need to remove the pwd from the prototype of the function
	ret = ft_strdup(pwd);
	i = 1;
	while (path[i])
	{
		up_a_parent(&ret, "helloo");
		i++;
	}
	return (ret);
}

char		*get_real_path(char *path, char *pwd)
{
	char	*ret;
	char	*tmp;

	if (*path == '/')
		return (ft_strdup(path));
	if (!same_char(path, '.'))
		return (parent_dots(path, pwd));
	// ret = ft_strdup(var_get_value("PATH", 1)); same here
	ret = ft_strdup(pwd);
	tmp = path;
	printf("%s             %s\n", ret, tmp);
	while (*tmp)
	{
		if (tmp[0] == '.' && tmp[1] == '.' && tmp[2] == '.')
			return (NULL);
		if (ft_strstr(tmp, "../") == tmp)
			tmp = up_a_parent(&ret, tmp);
		if (tmp[0] != '.' && tmp[0] != '/')
			tmp = new_path(&ret, tmp);
		tmp++;
		printf("%s             %s\n", ret, tmp);
	}
	return (ret);
}

int			main(void)
{
	printf("%s\n", get_real_path("......", "/Users/aminewalialami/42sh/trash/42sh_correction/42sh.txts"));
	return (0);
}
