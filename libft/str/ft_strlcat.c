/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 17:14:11 by adda-sil          #+#    #+#             */
/*   Updated: 2019/10/17 19:23:48 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstsize)
{
	char			*dest_ptr;
	char			*src_ptr;
	unsigned int	i;
	unsigned int	dest_len;

	dest_ptr = dest;
	src_ptr = (char *)src;
	i = dstsize;
	while (i-- != 0 && *dest_ptr != '\0')
		dest_ptr++;
	dest_len = dest_ptr - dest;
	i = dstsize - dest_len;
	if (i == 0)
		return (dest_len + ft_strlen(src_ptr));
	while (*src_ptr)
	{
		if (i != 1)
		{
			*dest_ptr++ = *src_ptr;
			i--;
		}
		src_ptr++;
	}
	*dest_ptr = 0;
	return (dest_len + (src_ptr - src));
}
