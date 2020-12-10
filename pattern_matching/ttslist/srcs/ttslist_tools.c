/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ttslist_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ozaazaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 09:54:01 by ozaazaa           #+#    #+#             */
/*   Updated: 2020/12/10 09:54:02 by ozaazaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ttslist.h"

void	*tts_memalloc(size_t size)
{
	unsigned char	*mem;
	size_t			i;

	i = 0;
	mem = malloc(size);
	if (!mem)
		return (NULL);
	while (i < size)
		mem[i++] = 0;
	return (mem);
}

void	tts_bzero(void *mem, size_t size)
{
	unsigned char	*bytes;
	size_t			i;

	i = 0;
	bytes = mem;
	while (i < size)
	{
		bytes[i] = 0;
		i++;
	}
}
