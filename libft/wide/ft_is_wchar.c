/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_wchar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 22:35:40 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/13 00:28:26 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wchar.h>
#include "libft.h"

int
	ft_is_wchar(unsigned char c)
{
	if ((c & 0b10000000) == 0b10000000)
		return (1);
	return (0);
}

int
	ft_is_fwchar(unsigned char c)
{
	if ((c & 0b11000000) == 0b11000000)
		return (1);
	return (0);
}
