/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strslice.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 17:48:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/02 18:06:35 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_strslice_buff(char *buffer, size_t start, size_t count)
{
	int copyed;

	copyed = ft_strcpy(&buffer[start], &buffer[start + count]);
	buffer[start + copyed] = 0;
}

char
	*ft_strslice(char *str, size_t start, size_t count)
{
	size_t	len;
	char	*new_str;
	int		copyed;

	len = ft_strlen(str);
	if (start + count > len)
		return (NULL);
	if (!(new_str = malloc(sizeof(char) * (len - count + 1))))
		return (NULL);

	copyed = ft_strcpy(&buffer[start], &buffer[start + count]);
	buffer[start + copyed] = 0;
	ft_strcpy(new_str);
}
