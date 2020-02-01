/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_charset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/14 08:05:27 by adda-sil          #+#    #+#             */
/*   Updated: 2019/10/23 18:53:00 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int			str_occur(char const *str, char *charset, int charset_len)
{
	char	*end;
	int		len;
	int		count;

	count = 0;
	while (*charset && (end = ft_strstr(str, charset)))
	{
		len = end - str;
		if (len > 0)
			count++;
		str = end + charset_len;
	}
	if (ft_strlen(str))
		count++;
	return (count);
}

static const char	*alloc_str(char **res, int i, const char *str, int len)
{
	int j;

	j = -1;
	if (!(res[i] = malloc(sizeof(char) * (len + 1))))
	{
		while (--i >= 0)
			free(res[i]);
		free(res);
		return (NULL);
	}
	while (++j < len)
		res[i][j] = str[j];
	res[i][j] = '\0';
	return (str + len + 1);
}

char				**ft_split_charset(char const *str, char *charset)
{
	char	*ptr_end;
	char	**res;
	int		len;
	int		i;

	if (!str || !charset)
		return (NULL);
	i = -1;
	len = str_occur(str, charset, 1);
	if (!(res = malloc(sizeof(char *) * (len + 1))))
		return (NULL);
	if (len > 0 && *charset)
	{
		while ((ptr_end = ft_strstr(str, charset)))
			if ((ptr_end - str > 0)
				&& !(str = alloc_str(res, ++i, str, (ptr_end - str))))
				return (NULL);
			else if ((ptr_end - str) == 0)
				str += 1;
	}
	if (len > 0 && ft_strlen(str))
		alloc_str(res, ++i, str, ft_strlen(str));
	res[i + 1] = 0;
	return (res);
}
