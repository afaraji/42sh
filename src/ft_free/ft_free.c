/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afaraji <afaraji@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 17:44:57 by afaraji           #+#    #+#             */
/*   Updated: 2020/10/30 17:45:01 by afaraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_21sh.h"
#include "../inc/builtins.h"
#include "../inc/parse.h"
#include "../inc/ast.h"
#include "../inc/exec.h"
#include "../inc/ft_free.h"
#include "../inc/readline.h"

void	free_tab(char **av)
{
	int i;

	i = 0;
	if (!av)
		return ;
	while (av[i])
	{
		ft_strdel(&av[i]);
		i++;
	}
	free(av);
	av = NULL;
}

void	free_list(t_l *head)
{
	t_l	*tmp1;
	t_l	*tmp2;

	tmp1 = head;
	tmp2 = NULL;
	while (tmp1)
	{
		tmp2 = tmp1;
		tmp1 = tmp1->next;
		ft_strdel(&tmp2->data);
		free(tmp2);
	}
}

void	free_l(t_l *list)
{
	if (!list)
		return ;
	free_l(list->next);
	ft_strdel(&(list->data));
	free(list);
	list = NULL;
}

void	free_tokens(t_list_token *tokens)
{
	if (!tokens)
		return ;
	free_tokens(tokens->next);
	tokens->is_ok = 0;
	tokens->type = 0;
	tokens->prec = NULL;
	tokens->next = NULL;
	if (tokens->data)
		ft_strdel(&(tokens->data));
	free(tokens);
	tokens = NULL;
}

void	free_cmd_list(t_cmdlist *list)
{
	if (!list)
		return ;
	free_cmd_list(list->next);
	free_and_or_list(list->and_or);
	free(list);
	list = NULL;
}
