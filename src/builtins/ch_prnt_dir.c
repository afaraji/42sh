/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ch_prnt_dir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 02:09:44 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/05 17:53:59 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/builtins.h"
#include "../../inc/expansion.h"

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

char		*get_real_path(char *path)
{
	char	*ret;
	char	*tmp;

	ret = ft_strdup(var_get_value("PATH", 1));
	tmp = path;
	printf("%s             %s\n", ret, tmp);
	while (*tmp)
	{
		if (tmp[0] == '.' && tmp[1] == '.' && tmp[2] == '.')
			return (ft_strdup(path));
		if (ft_strstr(tmp, "../") == tmp)
			tmp = up_a_parent(&ret, tmp);
		if (tmp[0] != '.' && tmp[0] != '/')
			tmp = new_path(&ret, tmp);
		tmp++;
		printf("%s             %s\n", ret, tmp);
	}
	return (ret);
}
