/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 17:18:44 by riblanc           #+#    #+#             */
/*   Updated: 2020/03/26 13:29:54 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

char	*get_substr(const char *s, unsigned int start, size_t len)
{
	char			*tmp;
	unsigned int	size;

	size = 0;
	while (*(s + size))
		++size;
	if (start > size)
	{
		if (!(tmp = malloc(sizeof(*tmp))))
			return (0);
		*tmp = '\0';
		return (tmp);
	}
	if (!s || !(tmp = malloc(sizeof(*tmp) * (len + 1))))
		return (0);
	*(tmp + len) = 0;
	while (len--)
		*(tmp + len) = *(s + start + len);
	return (tmp);
}
