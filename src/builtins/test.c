/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/18 14:35:44 by aminewalial       #+#    #+#             */
/*   Updated: 2020/12/03 00:21:03 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/test.h"

static int	one_arg(char **av)
{
	if (!av[1][0])
		return (1);
	else
		return (0);
}

static int	two_args(char **av)
{
	if (!ft_strcmp(av[1], "!"))
		return (!one_arg(av + 1));
	else
	{
		if (unary_operator_check(av[1]))
			return (2);
		else
			return (file_check(av[1][1], av[2]));
	}
	return (0);
}

static int	three_args(char **av)
{
	if (!ft_strcmp(av[1], "!"))
		return (!two_args(av + 1));
	else if (!ft_strcmp(av[1], "(") && !ft_strcmp(av[3], ")"))
	{
		if (av[2] && av[2][0])
			return (0);
	}
	else if (!binary_operator_check(av))
		return (expression_check(av));
	return (1);
}

static int	four_more(char **av)
{
	if (!ft_strcmp(av[1], "!") && !av[5])
		return (!three_args(av + 1));
	else
	{
		ft_putendl_fd("42sh: test: too many arguments", 2);
		return (2);
	}
}

int			ft_test(char *cmd, char **av)
{
	(void)cmd;
	if (!av[1])
		return (1);
	else if (av[1] && !av[2])
		return (one_arg(av));
	else if (av[2] && !av[3])
		return (two_args(av));
	else if (av[3] && !av[4])
		return (three_args(av));
	else if (av[4])
		return (four_more(av));
	return (0);
}
