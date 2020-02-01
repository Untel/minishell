/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 18:44:48 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/12 22:51:01 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdint.h>

size_t	ft_intlen(int64_t n)
{
	uint64_t	nt;
	size_t		length;

	nt = (n < 0 ? -n : n);
	length = (n < 0 ? 2 : 1);
	while ((nt /= 10) != 0)
		length++;
	return (length);
}

size_t	ft_uintlen(uint64_t nt)
{
	size_t		length;

	length = 1;
	while ((nt /= 10) != 0)
		length++;
	return (length);
}
