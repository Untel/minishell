/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pow.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 10:06:17 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/09 01:45:35 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <inttypes.h>

uint64_t	ft_pow(uint64_t nb, int power)
{
	int			i;
	uint64_t	count;

	count = nb;
	if (power < 0)
		return (0);
	if (power == 0)
		return (1);
	if (nb == 0)
		return (power == 0 ? 1 : 0);
	i = 0;
	while (++i < power)
		count *= nb;
	return (count);
}
