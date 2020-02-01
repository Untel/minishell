/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 13:08:30 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/06 22:08:27 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

char	*ft_uitoa(unsigned int n)
{
	int				length;
	char			*str;
	unsigned int	nt;

	length = 1;
	nt = n;
	while ((nt /= 10) != 0)
		length++;
	if (!(str = (char *)malloc(sizeof(char) * (length + 1))))
		return (NULL);
	str += length;
	*str = 0;
	nt = n;
	*--str = (nt % 10) + '0';
	while ((nt /= 10) != 0)
		*--str = (nt % 10) + '0';
	return (str);
}
