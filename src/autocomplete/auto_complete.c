/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_complete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sazouaka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/02 19:33:46 by sazouaka          #+#    #+#             */
/*   Updated: 2020/10/02 19:33:48 by sazouaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_21sh.h"
#include "../../inc/builtins.h"
#include "../../inc/parse.h"
#include "../../inc/ast.h"
#include "../../inc/exec.h"
#include "../../inc/ft_free.h"
#include "../../inc/readline.h"

char			**auto_completion_3(char **args, int i)
{
	if (args[i][1] == '{')
		return (var_search_1(args[i] + 1));
	else
		return (var_search_2(args[i] + 1));
}

char			**auto_completion_1(t_line *line)
{
	char	**result;
	char	**args;
	int		i;

	args = completion_split(line->str);
	result = NULL;
	i = 0;
	while (args[i + 1])
		i++;
	if (args[i][0] == '~' && args[i][1] == '/')
		get_home_path(&args[i]);
	if (args[i][0] == '$')
		result = auto_completion_3(args, i);
	else if (i == 0 && args[0][0] == '.')
		result = files_dirs_search(args[0]);
	else if (i != 0 || is_path(args[i]) != 0)
		result = files_dirs_search(args[i]);
	else
		result = cmd_search(args[0]);
	if (args)
		free_tab(args);
	return (result);
}

int				auto_completion_2(char **result, t_line *line)
{
	ft_putchar('\n');
	print_result(result, line);
	free_tab(result);
	return (2);
}

int				is_valid_tab(char **t)
{
	if (!t)
		return (0);
	if (t[0] == NULL)
	{
		free_tab(t);
		return (0);
	}
	return (1);
}

int				auto_completion(t_line *line)
{
	char	**result;
	int		j;

	result = auto_completion_1(line);
	if (!is_valid_tab(result))
		return (0);
	j = 0;
	while (result && result[j + 1])
		j++;
	if (j == 0)
	{
		line->str = completed_line(line->str, result[0]);
		free_tab(result);
		if (line->init_pos == line->row)
			display_line_from_begin(line);
		else
			display_line(line);
		move_curs_right(line);
		return (1);
	}
	else
		return (auto_completion_2(result, line));
	return (0);
}
