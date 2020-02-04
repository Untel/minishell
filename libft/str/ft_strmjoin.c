/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 22:12:16 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 02:35:13 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmjoin(int size, char **strs, char *sep)
{
	char	*res;
	int		i;
	int		j;
	int		k;
	int		len;

	len = ((size > 0 ? size - 1 : 0) * ft_strlen(sep)) + 1;
	i = -1;
	while (++i < size)
		len += ft_strlen(strs[i]);
	if (!(res = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = -1;
	while (++i < size)
	{
		k = -1;
		while (strs[i][++k])
			*res++ = strs[i][k];
		j = -1;
		if (i != size - 1)
			while (sep[++j])
				*res++ = sep[j];
	}
	*res++ = 0;
	return (res - len);
}
