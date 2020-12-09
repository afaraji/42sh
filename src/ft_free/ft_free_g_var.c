/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_g_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 17:44:57 by afaraji           #+#    #+#             */
/*   Updated: 2020/10/30 17:45:01 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"
#include "../../inc/jobs.h"

void	free_t_var(t_variable *list)
{
	if (!list)
		return ;
	free_t_var(list->next);
	ft_strdel(&(list->key));
	ft_strdel(&(list->value));
	free(list);
	list = NULL;
}

void	free_job_list(t_job **j)
{
	if (!j || !*j)
		return ;
	if ((*j)->next)
		free_job_list(&(*j)->next);
	free_job(j);
}

void	free_aliases(t_alias **alias)
{
	if (!alias || !*alias)
		return ;
	if ((*alias)->next)
		free_aliases(&(*alias)->next);
	ft_strdel(&((*alias)->key));
	ft_strdel(&((*alias)->sub));
	(*alias)->next = NULL;
	free((*alias));
	(*alias) = NULL;
	alias = NULL;
}

void	free_history_list(t_hist *list)
{
	if (!list)
		return ;
	if (list->next)
		free_history_list(list->next);
	ft_strdel(&(list->hist_str));
	list->next = NULL;
	list->prec = NULL;
	free(list);
	list = NULL;
}

void	free_g_var(void)
{
	free_t_var(g_var.var);
	free_aliases(&g_var.aliases);
	free_history_list(g_var.history);
	free_job_list(&g_var.job);
	if (g_var.cpy_past)
		ft_strdel(&(g_var.cpy_past));
}
