/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 17:43:17 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/08 18:15:15 by awali-al         ###   ########.fr       */
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
	// printf("%s\n", path);
	if (!path)
		return (1);
	oldpwd = get_pwd(env);
	if (chdir(path))
	{
		ft_print(STDERR, "cd: %s: no such file or diroctory\n", path);
		free(path);
		return (1);
	}
	if (oldpwd)
		change_pwd("OLDPWD", oldpwd);
	change_pwd("PWD", path);
	ft_putendl(path);
	free(path);
	if (oldpwd)
		free(oldpwd);
	return (0);
}

int		ft_cd_1(char *flag, char **env)
{
	char	*oldpwd;
	char	*pwd;

	pwd = NULL;
	if (!flag)
		pwd = get_var_from_tab(env, "HOME");
	else
		pwd = ft_strdup(flag);
	if (ft_pdenied(pwd))
		return (1);
	oldpwd = get_pwd(env);
	if (chdir(pwd))
	{
		ft_strdel(&oldpwd);
		return (1);
	}
	ft_strdel(&pwd);
	pwd = getcwd(NULL, 0);
	change_pwd("OLDPWD", oldpwd);
	change_pwd("PWD", pwd);
	ft_putendl(pwd);
	ft_strdel(&pwd);
	ft_strdel(&oldpwd);
	return (0);
}

int		ft_cd_2(char *flag)
{
	ft_putstr_fd("cd: no such file or directory: ", 2);
	ft_putendl_fd(flag, 2);
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
	ft_putendl(flag);
	if (oldcwd)
		free(oldcwd);
	free(flag);
	return (0);
}

int		ft_cd(char *flag, char **env)
{
	char	*path;
	int		typ;

	if (flag == NULL)
		return (ft_cd_home(env));
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
		ft_putstr_fd("cd: not a directory: ", 2);
		ft_putendl_fd(path, 2);
		ft_strdel(&path);
		return (1);
	}
	else if (typ == 3)
		return (ft_cd_3(path, env));
	return (0);
}
