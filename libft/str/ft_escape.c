/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_escape.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 22:00:49 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/12 17:24:44 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

size_t
	ft_escape(char *str, int escape)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == escape)
		{
			if (str[i + 1] == escape)
				i++;
			count++;
		}
		else
			str[i - count] = str[i];
		i++;
	}
	str[i - count] = 0;
	return (count);
}

size_t
	ft_escape_sequence(char *str, int escape,
		char *sequence, int jump)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == escape && (i == 0 || str[i - 1] != escape)
			&& ft_strchr(sequence, str[i + 1]))
			count += jump + 1;
		else
			str[i - count] = str[i];
		i++;
	}
	str[i - count] = 0;
	return (count);
}

size_t
	ft_escape_malloc(char **str, int escape)
{
	size_t	count;
	size_t	initial_len;
	char	*new_str;

	initial_len = ft_strlen(*str);
	count = ft_escape(*str, escape);
	if (count == 0)
		return (0);
	if (!(new_str = malloc(sizeof(char) * (initial_len - count + 1))))
		return (0);
	ft_strcpy(new_str, *str);
	free(*str);
	*str = new_str;
	return (count);
}
