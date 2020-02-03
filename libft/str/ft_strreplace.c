/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 23:30:52 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 23:34:10 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *
	ft_strreplace(char *buffer, char *target, char *replace)
{
	char *place;

	place = ft_strstr(buffer, target);
	place += ft_strcpy(place, replace);
}
