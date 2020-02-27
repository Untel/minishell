/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 16:26:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/27 13:56:02 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_atoi(const char *str)
{
	unsigned int	i;
	unsigned int	count;
	int				sign;

	i = 0;
	count = 0;
	sign = 1;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (ft_isdigit(str[i]))
	{
		if (i > 18 && str[i] > '7')
			return (sign == 1 ? -1 : 0);
		count = count * 10 + (str[i++] - '0');
	}
	return (count * sign);
}

int			ft_natoi(const char *str, unsigned int len)
{
	unsigned int	i;
	unsigned int	count;
	int				sign;

	i = 0;
	count = 0;
	sign = 1;
	while (i < len && ft_isspace(str[i]))
		i++;
	if (i < len && (str[i] == '-' || str[i] == '+'))
		if (str[i++] == '-')
			sign = -1;
	while (i < len && ft_isdigit(str[i]))
	{
		if (i > 18 && str[i] > '7')
			return (sign == 1 ? -1 : 0);
		count = count * 10 + (str[i++] - '0');
	}
	return (count * sign);
}

int			ft_rev_atoi_idx(const char *str, int i)
{
	if (i < 0)
		return (-1);
	while (i >= 0 && ft_isspace(str[i]))
		i--;
	while (i >= 0 && ft_isdigit(str[i]))
		i--;
	if (i >= 0 && (str[i] == '-' || str[i] == '+'))
		i--;
	return (ft_atoi(str + i));
}
