/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 17:15:23 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/07 22:04:37 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	ft_memdel(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

int		ft_imemdel(void **ptr)
{
	ft_memdel(ptr);
	return (1);
}

int		ft_memdel_false(void **ptr)
{
	ft_memdel(ptr);
	return (0);
}
