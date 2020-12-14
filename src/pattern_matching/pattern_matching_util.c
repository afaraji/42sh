/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching_util.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 10:36:21 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 11:20:51 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/pattern_matching.h"

int			is_dir(char *path)
{
	struct stat	s;

	if (stat(path, &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			return (1);
		}
	}
	return (0);
}

void		bubblesort(char **arr, int n)
{
	int	i;
	int	j;

	i = -1;
	j = -1;
	while (++i < n - 1)
		while (++j < n - i - 1)
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
				swap(&arr[j], &arr[j + 1]);
}

t_list_head	get_file_name(char *directory, unsigned char hidden)
{
	DIR				*dir;
	struct dirent	*diread;
	t_list_head		files;

	ttslist_init(&files);
	if ((dir = opendir(directory)) != NULL)
	{
		while ((diread = readdir(dir)) != NULL)
		{
			if (diread->d_name[0] != '.' || hidden)
			{
				files.push(&files, ft_strdup(diread->d_name));
			}
		}
		closedir(dir);
	}
	return (files);
}

t_list_head	get_file_names(char *directory, unsigned char hidden)
{
	t_list_head		files;
	char			**temp;

	files = get_file_name(directory, hidden);
	temp = list_to_tab_oz(&files);
	bubblesort(temp, files.size);
	ttslist_purge(&files, free);
	files = tab_to_list(temp);
	return (files);
}

int			merge_ttslist(t_list_head *list1, t_list_head *list2)
{
	void	*content;

	list2->iterator = list2->first;
	while ((content = ttslist_iter_content(list2)))
		list1->push(list1, ft_strdup(content));
	return (1);
}
