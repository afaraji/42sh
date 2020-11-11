/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 14:37:26 by aminewalial       #+#    #+#             */
/*   Updated: 2020/10/23 19:07:05 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include "libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>

int		my_test(char *cmd, char **av, char **env);
int		binary_operator_check(char **av);
int		file_check(char o, char *av);
int		unary_operator_check(char *av);
int		expression_check(char **av);
int		ft_isint(char *str);

#endif
