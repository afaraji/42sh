/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replaceword.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 18:18:50 by sazouaka          #+#    #+#             */
/*   Updated: 2020/12/13 20:54:47 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_21sh.h"
#include "../inc/builtins.h"
#include "../inc/parse.h"
#include "../inc/ast.h"
#include "../inc/exec.h"
#include "../inc/ft_free.h"
#include "../inc/readline.h"

char	*errlist(int err_no)
{
	static char	*s[19];

	if (!s[0])
	{
		s[0] = "Undefined error: 0";
		s[1] = "Operation not permitted";
		s[2] = "No such file or directory";
		s[3] = "No such process";
		s[4] = "Interrupted system call";
		s[5] = "Input/output error";
		s[6] = "Device not configured";
		s[7] = "Argument list too long";
		s[8] = "Exec format error";
		s[9] = "Bad file descriptor";
		s[10] = "No child processes";
		s[11] = "Resource deadlock avoided";
		s[12] = "Cannot allocate memory";
		s[13] = "Permission denied";
		s[14] = "Bad address";
		s[15] = "Block device required";
		s[16] = "Resource busy";
		s[17] = "File exists";
		s[18] = "Cross-device link";
	}
	return ((err_no < 19) ? s[err_no] : err_list_1(err_no - 19));
}

int		system_calls(char *func, int ret, int failure_value)
{
	extern int	errno;

	if (ret != failure_value)
		return (ret);
	ft_print(STDERR, "System call failure: %s: %s\n", func, errlist(errno));
	exit(errno);
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
		if (ft_strstr(&(s[i]), old) == &s[i])
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
		if (ft_strstr(s, old) == s)
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
