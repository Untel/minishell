/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlowcase.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 13:09:40 by adda-sil          #+#    #+#             */
/*   Updated: 2019/10/22 21:21:36 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strlowcase(const char *str)
{
	char	*ptr;
	size_t	len;

	len = ft_strlen(str);
	if (!(ptr = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (*str)
		*ptr++ = ft_tolower(*str++);
	*ptr = 0;
	return (ptr - len);
}
