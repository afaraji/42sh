/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_arithmetic.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arochdi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 02:26:28 by arochdi           #+#    #+#             */
/*   Updated: 2020/11/28 01:46:03 by arochdi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/expansion.h"

static long long int	do_op(long long int first_operand,
						long long int second_operand, char operator, int *err)
{
	if (operator == '+')
		return (first_operand + second_operand);
	else if (operator == '-')
		return (first_operand - second_operand);
	else if (operator == '/')
	{
		if (second_operand == 0)
			return (*err = 1);
		return (first_operand / second_operand);
	}
	else if (operator == '*')
		return (first_operand * second_operand);
	else if (operator == '%')
	{
		if (second_operand == 0)
			return (*err = 1);
		return (first_operand % second_operand);
	}
	else
		return (logical_do_op(first_operand, second_operand, operator));
	return (0);
}

static long long int	parenth_operation(char **str, int *err)
{
	long long int	nbr;
	int				signe;

	nbr = 0;
	signe = 1;
	if ((*str)[0] == '+' || (*str)[0] == '-')
	{
		if ((*str)[0] == '-')
			signe = -1;
		*str = *str + 1;
	}
	if ((*str)[0] == '(')
	{
		*str = *str + 1;
		nbr = get_first_operand(str, err);
		if ((*str)[0] == ')')
			*str = *str + 1;
		return (signe * nbr);
	}
	while ('0' <= (*str)[0] && (*str)[0] <= '9')
	{
		nbr = (nbr * 10) + (*str)[0] - '0';
		*str = *str + 1;
	}
	return (signe * nbr);
}

static long long int	operations(char **str, int *err)
{
	long long int	first_operand;
	long long int	second_operand;
	char			operator;

	first_operand = parenth_operation(str, err);
	while ((*str)[0] == '*' || (*str)[0] == '/' || (*str)[0] == '%'
			|| ((*str)[0] == '=' && (*str)[1] == '=')
			|| ((*str)[0] == '>' && (*str)[1] == '=')
			|| ((*str)[0] == '<' && (*str)[1] == '=')
			|| (*str)[0] == '>' || (*str)[0] == '<')
	{
		logical_operator(&operator, *str);
		operator = operator >= 0 ? (*str)[0] : operator;
		*str = operator < -2 ? *str + 2 : *str + 1;
		second_operand = parenth_operation(str, err);
		first_operand = do_op(first_operand, second_operand, operator, err);
	}
	return (first_operand);
}

long long int			get_first_operand(char **str, int *err)
{
	long long int	first_operand;
	long long int	second_operand;
	char			operator;

	first_operand = parenth_operation(str, err);
	while ((*str)[0] != '\0' && (*str)[0] != ')')
	{
		logical_operator(&operator, *str);
		operator = operator >= 0 ? (*str)[0] : operator;
		*str = operator < -2 ? *str + 2 : *str + 1;
		if (operator < 0)
			second_operand = operations(str, err);
		else if (operator == '+' || operator == '-')
			second_operand = operations(str, err);
		else
			second_operand = parenth_operation(str, err);
		first_operand = do_op(first_operand, second_operand, operator, err);
	}
	return (first_operand);
}

int						expans_arith(char **operation)
{
	char	*result;
	int		err;

	err = 0;
	if (ft_strlen(*operation) == 0)
		*operation = ft_strdup("0");
	else
	{
		ft_move_space(operation);
		result = ft_iitoa(get_first_operand(operation, &err));
		if (err)
			return (1);
		*operation = ft_strdup(result);
	}
	return (0);
}
