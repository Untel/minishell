/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strslice.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:48:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/02 18:51:05 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char
	*ft_strslice(char *str, size_t start, size_t count)
{
	char	*new_str;
	size_t	i;

	i = ft_strlen(str);
	if (start + count > i)
		return (NULL);
	if (!(new_str = malloc(sizeof(char) * (i - count + 1))))
		return (NULL);
	i = 0;
	while (i < start)
		new_str[i] = str[i];
	i += ft_strcpy(new_str + i, str + i + count);
	free(str);
	return (new_str);
}
