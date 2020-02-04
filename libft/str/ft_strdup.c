/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 17:35:06 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 09:41:24 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*mal;
	size_t	size;

	size = ft_strlen(src);
	if (!(mal = malloc(sizeof(char) * (size + 1))))
		return (NULL);
	mal[size] = 0;
	while (size--)
		mal[size] = src[size];
	return (mal);
}

char	*ft_strndup(const char *src, size_t n)
{
	char	*mal;
	size_t	size;

	size = ft_strlen(src);
	size = size > n ? n : size;
	if (!(mal = malloc(sizeof(char) * (size + 1))))
		return (NULL);
	mal[size] = 0;
	while (size--)
		mal[size] = src[size];
	return (mal);
}

char	*ft_strdup_until(const char *src, char c)
{
	char	*ptr;

	ptr = ft_strchr(src, c);
	if (!ptr)
		return (ft_strdup(src));
	else
		return (ft_strndup(src, ptr - src));
}
