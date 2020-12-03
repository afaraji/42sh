/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/19 08:45:33 by aminewalial       #+#    #+#             */
/*   Updated: 2020/12/03 03:02:34 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/test.h"

static int	ft_isint(char *str)
{
	int		i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int			unary_operator_check(char *av)
{
	if (!av[2] && ((av[1] > 'a' && av[1] < 'h') || av[1] == 'L' || av[1] == 'p'
			|| av[1] == 'r' || av[1] == 'S' || av[1] == 's' || av[1] == 'u' ||
			av[1] == 'w' || av[1] == 'x' || av[1] == 'z'))
		return (0);
	else
	{
		ft_putstr_fd("42sh: test: ", 2);
		ft_putstr_fd(av, 2);
		ft_putendl_fd(": unary operator expected", 2);
		return (1);
	}
}

int			file_check(char o, char *av)
{
	struct stat	file;
	int			ret;

	ret = 1;
	ret = !lstat(av, &file) && o == 'e' ? 0 : ret;
	ret = S_ISBLK(file.st_mode) && o == 'b' ? 0 : ret;
	ret = S_ISCHR(file.st_mode) && o == 'c' ? 0 : ret;
	ret = S_ISDIR(file.st_mode) && o == 'd' ? 0 : ret;
	ret = S_ISREG(file.st_mode) && o == 'f' ? 0 : ret;
	ret = S_ISLNK(file.st_mode) && o == 'L' ? 0 : ret;
	ret = S_ISFIFO(file.st_mode) && o == 'p' ? 0 : ret;
	ret = S_ISSOCK(file.st_mode) && o == 'S' ? 0 : ret;
	ret = (file.st_mode & S_ISUID) && o == 'u' ? 0 : ret;
	ret = (file.st_mode & S_ISGID) && o == 'g' ? 0 : ret;
	ret = (file.st_mode & S_IRUSR) && o == 'r' ? 0 : ret;
	ret = (file.st_mode & S_IWUSR) && o == 'w' ? 0 : ret;
	ret = (file.st_mode & S_IXUSR) && o == 'x' ? 0 : ret;
	ret = (file.st_size > 0) && o == 's' ? 0 : ret;
	ret = !ft_strlen(av) && o == 'z' ? 0 : ret;
	return (ret);
}

int			binary_operator_check(char **av)
{
	if (!ft_strcmp(av[2], "=") || !ft_strcmp(av[2], "!=") ||
			!ft_strcmp(av[2], "-eq") || !ft_strcmp(av[2], "-ne") ||
			!ft_strcmp(av[2], "-gt") || !ft_strcmp(av[2], "-ge") ||
			!ft_strcmp(av[2], "-lt") || !ft_strcmp(av[2], "-le"))
		return (0);
	else
	{
		ft_putstr_fd("42sh: test: ", 2);
		ft_putstr_fd(av[2], 2);
		ft_putendl_fd(": binary operator expected", 2);
		return (1);
	}
}

int			expression_check(char **av)
{
	int			r;

	if (ft_strcmp(av[2], "=") && ft_strcmp(av[2], "!=") &&
			(!ft_isint(av[1]) || !ft_isint(av[3])))
	{
		ft_putstr_fd("42sh: test: ", 2);
		if (!ft_isint(av[1]))
			ft_putstr_fd(av[1], 2);
		else
			ft_putstr_fd(av[3], 2);
		ft_putendl_fd(": integer expression expected", 2);
		return (2);
	}
	r = 1;
	r = !ft_strcmp(av[2], "=") && !ft_strcmp(av[1], av[3]) ? 0 : r;
	r = !ft_strcmp(av[2], "!=") && ft_strcmp(av[1], av[3]) ? 0 : r;
	r = !ft_strcmp(av[2], "-eq") && ft_atoi(av[1]) == ft_atoi(av[3]) ? 0 : r;
	r = !ft_strcmp(av[2], "-ne") && ft_atoi(av[1]) != ft_atoi(av[3]) ? 0 : r;
	r = !ft_strcmp(av[2], "-gt") && ft_atoi(av[1]) > ft_atoi(av[3]) ? 0 : r;
	r = !ft_strcmp(av[2], "-ge") && ft_atoi(av[1]) >= ft_atoi(av[3]) ? 0 : r;
	r = !ft_strcmp(av[2], "-lt") && ft_atoi(av[1]) < ft_atoi(av[3]) ? 0 : r;
	r = !ft_strcmp(av[2], "-le") && ft_atoi(av[1]) <= ft_atoi(av[3]) ? 0 : r;
	return (r);
}
