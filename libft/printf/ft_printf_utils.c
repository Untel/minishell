/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 20:20:55 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/12 22:39:08 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

int
	ft_is_conv(char *str, char c)
{
	while (*str)
		if (*str++ == c)
			return (1);
	return (0);
}

char
	*ft_add_sign(char sign, char *str)
{
	char *res;

	if (!(res = malloc(sizeof(char) * (ft_strlen(str) + 2))))
		return (NULL);
	*res = sign;
	ft_strcpy(&res[1], str);
	return (res);
}

void
	*ft_f(void *ptr, void *res)
{
	ft_memdel(&ptr);
	return (res);
}

char
	*ft_fill(char *str, size_t count, char c, int align_left)
{
	size_t	initial_len;
	size_t	i;
	char	*new_str;

	i = 0;
	if ((initial_len = ft_strlen(str)) >= count)
		return (ft_strdup(str));
	if (!(new_str = malloc(sizeof(char) * (count + 1))))
		return (NULL);
	while (!align_left && i < (count - initial_len))
		new_str[i++] = c;
	while (*str)
		new_str[i++] = *str++;
	while (align_left && i < count)
		new_str[i++] = ' ';
	new_str[i] = 0;
	return (new_str);
}

char
	*ft_fill_c(char cc, size_t count, char c, int align_left)
{
	size_t	i;
	char	*new_str;

	i = 0;
	count = count > 1 ? count : 1;
	if (!(new_str = malloc(sizeof(char) * (count + 1))))
		return (NULL);
	while (!align_left && i < (count - 1))
		new_str[i++] = c;
	new_str[i++] = cc;
	while (align_left && i < count)
		new_str[i++] = ' ';
	new_str[i] = 0;
	return (new_str);
}
