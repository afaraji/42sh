/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replaceword.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <sazouaka@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 18:18:50 by sazouaka          #+#    #+#             */
/*   Updated: 2020/11/20 18:18:52 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_21sh.h"
#include "../inc/builtins.h"
#include "../inc/parse.h"
#include "../inc/ast.h"
#include "../inc/exec.h"
#include "../inc/ft_free.h"
#include "../inc/readline.h"

int		word_count(char *s, char *old)
{
	int	cnt;
	int	i;
	int	oldlen;
  
	cnt = 0;
	i = 0;
	oldlen = ft_strlen(old);
	while (s[i] != '\0')
	{
		if (strstr(&(s[i]), old) == &s[i])
		{ 
			cnt++; 
			i += oldlen - 1; 
		} 
		i++;
	}
	return (cnt);
}

char	*ft_replaceword(char *s, char *old, char *new)
{ 
	char	*result; 
	int		i;
	int		cnt;
	int		oldlen;
	int		newlen;

	cnt = word_count(s, old);
	newlen = ft_strlen(new);
	oldlen = ft_strlen(old);
	result = (char *)malloc(ft_strlen(s) + cnt * (newlen - oldlen) + 1);
	i = 0;
	while (*s)
	{
		if (strstr(s, old) == s)
		{
			ft_strcpy(&result[i], new);
			i += newlen;
			s += oldlen;
		}
		else
			result[i++] = *s++;
	}
	result[i] = '\0';
	return (result);
}