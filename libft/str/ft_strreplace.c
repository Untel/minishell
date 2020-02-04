/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 23:30:52 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 05:19:57 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char
	*ft_strreplace(char *buffer, char *target, char *replace)
{
	char	*place;
	char	*tmp;
	int		len;

	place = ft_strstr(buffer, target);
	if (place)
	{
		len = ft_strlen(target);
		tmp = ft_strdup(place + len);
		place += ft_strcpy(place, replace);
		place += ft_strcpy(place, tmp);
		free(tmp);
		*place = 0;
	}
	return (buffer);
}
