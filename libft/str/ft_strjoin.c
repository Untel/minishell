/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/11 22:12:16 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/09 13:44:47 by adda-sil         ###   ########.fr       */
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

	if (!s1 && !s2)
		return (NULL);
	if (!s1 || !s2)
		return (!s1 ? ft_strdup(s2) : ft_strdup(s1));
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

char	*ft_strjoin_sep(char const *s1, char const *s2, char sep)
{
	char	*str;
	char	*ptr;
	int		l1;
	int		l2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 || !s2)
		return (!s1 ? ft_strdup(s2) : ft_strdup(s1));
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	if (!(str = malloc(sizeof(char) * (l1 + l2 + 2))))
		return (NULL);
	ptr = str;
	while (l1--)
		*str++ = *s1++;
	*str++ = sep;
	while (l2--)
		*str++ = *s2++;
	*str = 0;
	return (ptr);
}
