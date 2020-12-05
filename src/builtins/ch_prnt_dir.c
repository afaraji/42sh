/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ch_prnt_dir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/05 02:09:44 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/05 03:12:32 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
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

char		*get_prnt_dir(char *path)
{
	char	*buf;
	char	*tmp;
	char	*ret;

	if (path[0] == '/')
		return (ft_strdup(path));
	// ret = ft_strdup(getcwd(NULL, 0)); when trying it with the real deal replace the line below with this one
	ret = ft_strdup("/usr/bin");
	tmp = path;
	while (tmp[0] == '.' || tmp[0] == '/')
	{
		if (tmp[0] == '.' && tmp[1] == '.' && tmp[2] == '.')
			return (NULL);
		if (ft_strstr(tmp, "../") == tmp)
			tmp = up_a_parent(&ret, tmp);
		tmp++;
	}
	if (!ft_strcmp(ret, "/") )
		buf = ft_strdup(ret);
	else
		buf = ft_strjoin(ret, "/");
	ft_strdel(&ret);
	ret = ft_strjoin(buf, tmp);
	ft_strdel(&buf);
	return (ret);
}

int			main(void)
{
	printf("%s\n", get_prnt_dir(".././///.././../Users"));
	return (0);
}
