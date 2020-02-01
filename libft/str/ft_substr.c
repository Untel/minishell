/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 15:31:45 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/13 00:54:15 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	int		slen;
	int		delta;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	delta = (slen - (start));
	if (delta > 0 && delta <= slen)
		slen = len;
	else if (delta > 0 && delta > slen)
		slen = delta;
	else
		slen = 0;
	if (!(str = (char *)malloc(sizeof(char) * (slen + 1))))
		return (NULL);
	str[slen] = 0;
	while (--slen >= 0)
		str[slen] = s[start + slen];
	return (str);
}

char	*ft_subwstr(char const *s, unsigned int start, size_t len)
{
	size_t	i;

	while (start && s[start] && ft_is_wchar(start) && !ft_is_fwchar(start))
		start--;
	i = start + len;
	while (i && s[i] && ft_is_wchar(s[i]) && !ft_is_fwchar(s[i]))
		i--;
	return (ft_substr(s, start, (i - start)));
}
