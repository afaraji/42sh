/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 17:43:17 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/07 16:55:27 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

int		ft_cd_home(char **env)
{
	char	*path;
	char	*oldpwd;

	path = get_var_from_tab(env, "HOME");
	if (!path)
		return (1);
	oldpwd = get_pwd(env);
	if (chdir(path))
	{
		ft_print(STDERR, "cd: %s: no such file or diroctory", path);
		free(path);
		return (1);
	}
	if (oldpwd)
		change_pwd("OLDPWD", oldpwd);
	change_pwd("PWD", path);
	ft_putstr(path);
	ft_putchar('\n');
	free(path);
	if (oldpwd)
		free(oldpwd);
	return (0);
}

int		ft_cd_1(char *flag, char **env)
{
	char	*oldpwd;

	if (ft_pdenied(flag))
		return (1);
	oldpwd = get_pwd(env);
	if (chdir(flag))
	{
		ft_strdel(&oldpwd);
		return (1);
	}
	change_pwd("OLDPWD", oldpwd);
	change_pwd("PWD", flag);
	ft_putstr(flag);
	ft_putchar('\n');
	ft_strdel(&oldpwd);
	free(flag);
	return (0);
}

int		ft_cd_2(char *flag)
{
	ft_putstr_fd("cd: no such file or directory: ", 2);
	ft_putstr_fd(flag, 2);
	ft_putchar('\n');
	free(flag);
	return (1);
}

int		ft_cd_3(char *flag, char **env)
{
	char	*oldcwd;

	if (ft_pdenied(flag))
		return (1);
	oldcwd = get_pwd(env);
	if (chdir(flag))
	{
		if (oldcwd)
			free(oldcwd);
		return (1);
	}
	if (oldcwd)
		change_pwd("OLDPWD", oldcwd);
	change_pwd("PWD", flag);
	ft_putstr(flag);
	ft_putchar('\n');
	if (oldcwd)
		free(oldcwd);
	free(flag);
	return (0);
}

int		ft_cd(char *flag, char **env)
{
	char	*path;
	int		typ;

	if (ft_strcmp(flag, ".") == 0)
		return (0);
	path = get_real_path(flag);
	typ = verify_type(path);
	if (typ == 1)
		return (ft_cd_1(path, env));
	else if (typ == -1)
		return (ft_cd_2(path));
	else if (typ == 2)
	{
		ft_putstr("cd: not a directory: ");
		ft_putstr(path);
		ft_putchar('\n');
		ft_strdel(&path);
		return (1);
	}
	else if (typ == 3)
		return (ft_cd_3(path, env));
	return (0);
}
