/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_tab_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awali-al <awali-al@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 16:37:17 by afaraji           #+#    #+#             */
/*   Updated: 2020/12/10 18:51:59 by awali-al         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_21sh.h"
#include "../inc/builtins.h"
#include "../inc/parse.h"
#include "../inc/ast.h"
#include "../inc/exec.h"
#include "../inc/ft_free.h"
#include "../inc/readline.h"

static int	cnt_parts(const char *s, char *c)
{
	int		cnt;
	int		in_substring;

	in_substring = 0;
	cnt = 0;
	while (*s != '\0')
	{
		if (in_substring == 1 && is_char_in_str(*s, c))
			in_substring = 0;
		if (in_substring == 0 && !is_char_in_str(*s, c))
		{
			in_substring = 1;
			cnt++;
		}
		s++;
	}
	return (cnt);
}

static int	wlen(const char *s, char *c)
{
	int		len;

	len = 0;
	while (!is_char_in_str(*s, c) && *s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}

char		**strsplit_str(char const *s, char *c)
{
	char	**t;
	int		nb_word;
	int		index;

	if (!s)
		return (NULL);
	index = 0;
	nb_word = cnt_parts((const char *)s, c);
	t = (char **)malloc(sizeof(*t) * (nb_word + 1));
	if (t == NULL)
		return (NULL);
	while (nb_word--)
	{
		while (is_char_in_str(*s, c) && *s != '\0')
			s++;
		t[index] = ft_strsub((const char *)s, 0, wlen((const char *)s, c));
		if (t[index] == NULL)
			return (NULL);
		s = s + wlen(s, c);
		index++;
	}
	t[index] = NULL;
	return (t);
}
