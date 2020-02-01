/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 17:35:06 by adda-sil          #+#    #+#             */
/*   Updated: 2019/10/23 18:39:29 by adda-sil         ###   ########.fr       */
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
	while (*src)
		*mal++ = *src++;
	*mal = 0;
	return (mal - size);
}
