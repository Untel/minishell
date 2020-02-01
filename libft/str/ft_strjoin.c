/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 22:12:16 by adda-sil          #+#    #+#             */
/*   Updated: 2019/10/21 18:39:12 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*ptr;
	int		l1;
	int		l2;

	if (!s1 || !s2)
		return (NULL);
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	if (!(str = malloc(sizeof(char) * (l1 + l2 + 1))))
		return (NULL);
	ptr = str;
	while (l1--)
		*str++ = *s1++;
	while (l2--)
		*str++ = *s2++;
	*str = 0;
	return (ptr);
}
