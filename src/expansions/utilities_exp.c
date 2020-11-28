/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_exp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 02:05:18 by arochdi           #+#    #+#             */
/*   Updated: 2020/11/28 02:05:53 by arochdi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/expansion.h"

char	*var_get_value(char *var, int typ)
{
	int     i;
	char    **env;
	char    *tmp;

	i = 0;
	env = env_to_tab(g_var.var, typ);
	while(env[i++])
	{
		if (!ft_strncmp(var, env[i], ft_strlen(var)))
		{
			tmp = (env[i] + ft_strlen(var) + 1);
			free_tab(env);
			return (tmp);
		}	   
	}
	tmp = "";
	return (tmp);
}

static void		ft_strdel_dispatcher(char *s1, char *s2, char p)
{
	if (p == 1)
		free(s1);
	else if (p == 2)
		free(s2);
	else if (p == 3)
	{
		free(s1);
		free(s2);
	}
}

char			*ft_strjoin_free(char *s1, char *s2, int p)
{
	char	*joint;
	int		slen1;
	int		slen2;
	int		i;
	int		j;

	i = -1;
	slen1 = ft_strlen(s1);
	slen2 = ft_strlen(s2);
	joint = (char *)malloc(sizeof(char) * (slen1 + slen2 + 1));
	if (joint == NULL)
		return (NULL);
	while (++i < slen1)
		joint[i] = s1[i];
	j = 0;
	while (j < slen2)
	{
		joint[i] = s2[j];
		i++;
		j++;
	}
	joint[i] = '\0';
	ft_strdel_dispatcher(s1, s2, p);
	return (joint);
}

char	*ft_strndup(const char *src, size_t n)
{
	char	*dup;
	size_t	srclen;
	char	*dupdup;

	if (!src)
		return (NULL);
	srclen = ft_strlen(src);
	dup = malloc(sizeof(char) * (srclen + 1));
	if (dup == NULL)
		return (NULL);
	dupdup = dup;
	while (*src && n > 0)
	{
		*dup = *src;
		src++;
		dup++;
        n--;
	}
	*dup = '\0';
	return (dupdup);
}
