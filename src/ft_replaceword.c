/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replaceword.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 18:18:50 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/10 11:26:44 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_21sh.h"
#include "../inc/builtins.h"
#include "../inc/parse.h"
#include "../inc/ast.h"
#include "../inc/exec.h"
#include "../inc/ft_free.h"
#include "../inc/readline.h"

int		system_calls(char *func, int ret, int failure_value)
{
	extern const char *const	sys_errlist[];
	extern int					errno;

	if (ret != failure_value)
		return (ret);
	ft_print(STDERR, "System call failure: %s: %s\n", func, sys_errlist[errno]);
	exit(42);
}

void	*ft_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		system_calls("malloc: ", 0, 0);
	}
	return (ptr);
}

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
	int		oldlen;
	int		newlen;

	if (!old || !new)
		return (ft_strdup(s));
	i = word_count(s, old);
	newlen = ft_strlen(new);
	oldlen = ft_strlen(old);
	result = (char *)malloc(ft_strlen(s) + i * (newlen - oldlen) + 1);
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
