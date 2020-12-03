/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 14:37:26 by aminewalial       #+#    #+#             */
/*   Updated: 2020/12/03 00:20:47 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>

int		ft_test(char *cmd, char **av);
int		binary_operator_check(char **av);
int		file_check(char o, char *av);
int		unary_operator_check(char *av);
int		expression_check(char **av);

#endif
