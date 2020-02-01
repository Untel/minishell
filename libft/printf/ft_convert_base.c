/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/12 01:33:08 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/12 18:44:19 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>

static int
	ft_nb_char_base(uint64_t nbr, int base_size)
{
	if (nbr > 0)
		return (1 + (ft_nb_char_base(nbr / base_size, base_size)));
	else
		return (0);
}

char
	*ft_nbrbase(uint64_t nb, char *base, unsigned int base_size)
{
	char	*ptr;
	char	*save;
	int		len;

	len = nb > 0 ? (ft_nb_char_base(nb, base_size)) : 1;
	if (!(ptr = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	if (!ptr || ptr == NULL)
		return (0);
	save = ptr + len;
	*save-- = '\0';
	if (nb == 0)
		*save-- = base[nb % base_size];
	while (nb > 0)
	{
		*save-- = base[nb % base_size];
		nb /= base_size;
	}
	return (ptr);
}
