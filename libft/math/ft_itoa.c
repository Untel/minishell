/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 13:08:30 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/13 00:47:44 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_itoa(int n)
{
	return (ft_itoa_wrapper(n < 0 ? (uint64_t)-n : n, 0, n < 0));
}

char	*ft_llitoa(int64_t n)
{
	return (ft_itoa_wrapper(n < 0 ? (uint64_t)-n : n, 0, n < 0));
}

char	*ft_lluitoa(uint64_t n)
{
	return (ft_itoa_wrapper(n, 0, 0));
}

char	*ft_lluitoa_sep(uint64_t n, char sep)
{
	return (ft_itoa_wrapper(n, sep, 0));
}

char	*ft_itoa_wrapper(uint64_t n, char sep, int is_neg)
{
	size_t		length;
	char		*str;
	int			i;

	length = ft_uintlen(n);
	if (sep)
		length += ((length % 3 == 0 ? -1 : 0) + (length / 3));
	if (is_neg)
		length++;
	if (!(str = (char *)malloc(sizeof(char) * (length + 1))))
		return (NULL);
	str += length;
	*str = 0;
	i = 1;
	*--str = (n % 10) + '0';
	while ((n /= 10))
	{
		if (sep && (i++ % 3) == 0)
			*--str = sep;
		*--str = (n % 10) + '0';
	}
	if (is_neg)
		*--str = '-';
	return (str);
}
