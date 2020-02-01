/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_spaces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/14 08:05:27 by adda-sil          #+#    #+#             */
/*   Updated: 2019/10/23 15:43:10 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_split_spaces(char const *str)
{
	char *spaces;

	spaces = "\t\n\v\f\r ";
	return (ft_split_charset(str, spaces));
}
