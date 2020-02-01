/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 02:22:09 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/09 01:50:02 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strcat(char *dest, char *src)
{
	char			*dest_ptr;

	dest_ptr = dest;
	while (*dest)
		dest++;
	while ((*dest++ = *src++))
		;
	*dest = '\0';
	return (dest_ptr - dest);
}
