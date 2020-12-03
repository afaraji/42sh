/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 17:11:01 by awali-al          #+#    #+#             */
/*   Updated: 2020/12/03 00:26:41 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_TABLE_H
# define HASH_TABLE_H

# include "../libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <stdio.h>

# define HASH_BUFF 1000
# define HASH_FILE "/tmp/.hash"
# define SEPARATOR -2

char	*ft_strstich(char *str1, char thread, char *str2);
char	*ft_strappend(char *str, char c);
int		ft_str2del(char **str);

char	**hash_table(void);
char	*hash_chck(char *cmd);
int		hash_add(char *cmd, char *path);
int		get_fd(int o);
int		ft_hash(char *cmd, char **av);

#endif
