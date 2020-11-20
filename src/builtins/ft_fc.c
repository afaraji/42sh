/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <sazouaka@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 18:18:50 by sazouaka          #+#    #+#             */
/*   Updated: 2020/11/20 18:18:52 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

void	open_vim()
{
	
}

void	fc_l()
{
	//list all history commands if less than 16 command

	//list 16 comands from history if they are more than 16


	/* 
	output : 
	1      exit
	2      fc -l
	*/
}

void	ft_fc(char **flag)
{
	if (flag[1] == NULL)
	{
		open_vim();//to edit the last commande in history
	}
	else if ((ft_strequ(flag[1], "-l")))
	{
		fc_l();
	}
	else if ((ft_strequ(flag[1], "-r")))
	{
		fc_r();
	}
	else if ((ft_strequ(flag[1], "-n")))
	{
		fc_n();
	}
	else if ((ft_strequ(flag[1], "-e")))
	{
		fc_e();
	}
	else if ((ft_strequ(flag[1], "-s")))
	{
		fc_s();
	}
}
