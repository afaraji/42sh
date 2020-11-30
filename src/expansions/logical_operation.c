/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_operation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 19:24:28 by arochdi           #+#    #+#             */
/*   Updated: 2020/03/11 19:24:29 by arochdi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/expansion.h"

int		dollared(char *word)
{
	int		i;

	i = -1;
	while (word[++i])
		if (word[i] == '$')
			return (1);
	return (0);
}

void	logical_operator(char *operator, char *str)
{
	if (str[0] == '<' && str[1] != '=')
		*operator = -1;
	else if (str[0] == '>' && str[1] != '=')
		*operator = -2;
	else if (str[0] == '<' && str[1] == '=')
		*operator = -3;
	else if (str[0] == '>' && str[1] == '=')
		*operator = -4;
	else if (str[0] == '=' && str[1] == '=')
		*operator = -5;
	else if (str[0] == '&' && str[1] == '&')
		*operator = -6;
	else if (str[0] == '|' && str[1] == '|')
		*operator = -7;
	else
		*operator = 1;
}

int		logical_do_op(long long int first_operand,
					long long int second_operand, char operator)
{
	if (operator == -1)
		return (first_operand < second_operand);
	if (operator == -2)
		return (first_operand > second_operand);
	if (operator == -3)
		return (first_operand <= second_operand);
	if (operator == -4)
		return (first_operand >= second_operand);
	if (operator == -5)
		return (first_operand == second_operand);
	if (operator == -6)
		return (first_operand && second_operand);
	if (operator == -7)
		return (first_operand || second_operand);
	return (0);
}
