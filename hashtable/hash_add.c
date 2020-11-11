/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 19:20:08 by awali-al          #+#    #+#             */
/*   Updated: 2020/11/08 18:45:08 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash_table.h"

int		hash_add(char *cmd, char *path)
{
	char	*add;
	char	*tmp;
	int		fd;

	if ((fd = get_fd('a')) == -1)
		return (1);
	tmp = ft_strstich(cmd, '=', path);
	add = ft_strappend(tmp, SEPARATOR);
	ft_strdel(&tmp);
	write(fd, add, ft_strlen(add));
	ft_strdel(&add);
	close(fd);
	return (0);
}
