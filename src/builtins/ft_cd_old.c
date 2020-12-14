/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_old.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/01 17:48:13 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/14 14:20:20 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"
#include "../../inc/expansion.h"

int	ft_cd_old(char **env)
{
	char	*cwd;
	char	*tmp;

	tmp = ft_strdup(var_get_value("OLDPWD", 1));
	if (!tmp)
		return (1);
	cwd = get_pwd(env);
	if (!chdir(tmp))
	{
		change_pwd("PWD", tmp);
		ft_print(STDOUT, "%s\n", tmp);
		if (cwd)
			change_pwd("OLDPWD", cwd);
		free(tmp);
		free(cwd);
		return (0);
	}
	if (access(tmp, F_OK) == 0 && access(tmp, X_OK) == -1)
	{
		ft_print(STDERR, "cd: permission denied: %s\n", tmp);
		return (0);
	}
	ft_print(STDERR, "cd: %s: No such file or directory.\n", tmp);
	free(tmp);
	return (1);
}
