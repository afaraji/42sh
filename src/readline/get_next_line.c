/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 13:48:40 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/10 11:24:04 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

void		ft_get_index(char *str, int *i)
{
	*i = 0;
	while (str[*i] != '\n' && str[*i] != '\0')
		(*i)++;
}

static int	getting_out(char **str)
{
	*str ? ft_strdel(str) : 0;
	return (0);
}

int			get_next_line(const int fd, char **line)
{
	char			buff[BUFF_SIZE + 1];
	static	char	*str;
	char			*tmp;
	int				i;

	if (fd < 0 || read(fd, buff, 0) < 0 || line == NULL)
		return (-1);
	(str == NULL) ? (str = ft_strdup("")) : 0;
	while ((i = read(fd, buff, BUFF_SIZE)))
	{
		buff[i] = '\0';
		tmp = str;
		str = ft_strjoin(str, buff);
		ft_strdel(&tmp);
		if (ft_strchr(str, '\n'))
			break ;
	}
	if (ft_strlen(str) == 0 && i == 0)
		return (getting_out(&str));
	ft_get_index(str, &i);
	*line = ft_strsub(str, 0, i);
	tmp = ft_strdup(str + i + 1);
	ft_strdel(&str);
	str = tmp;
	return (1);
}
