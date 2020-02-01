/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsechar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 17:07:39 by adda-sil          #+#    #+#             */
/*   Updated: 2020/01/19 20:02:43 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include <wchar.h>

int
	ft_parse_string(char **buff, t_mod mods, va_list args)
{
	wchar_t	*ptr;
	char	*res;
	size_t	len;
	char	b[BUFFER_SIZE];

	*b = 0;
	res = NULL;
	mods.size = mods.conv == 'S' ? L : mods.size;
	if (mods.size != L)
		res = va_arg(args, char *);
	else
	{
		ptr = va_arg(args, wchar_t *);
		if (ptr && (len = ft_wchars_to_str(b, ptr)) == -1)
			return (-1);
	}
	res = mods.size == L ? b : res;
	res = ft_strdup(res ? res : "(null)");
	if (mods.precision > -1)
		res = ft_f(res, ft_subwstr(res, 0, mods.precision));
	if (!res || !(res = ft_f(res,
		ft_fill(res, mods.padding, mods.padchar, mods.align_left))))
		return (-1);
	*buff = res;
	return (ft_strlen(res));
}

int
	ft_parse_char(char **buff, t_mod mods, va_list args)
{
	int		c;
	char	*res;
	int		len;
	char	b[BUFFER_SIZE];

	*b = 0;
	len = 0;
	mods.size = (ft_isupper(mods.conv) && mods.size == N) ? L : mods.size;
	c = ft_is_conv("cC", mods.conv) ? va_arg(args, int) : mods.conv;
	len = mods.padding > 1 ? mods.padding : 1;
	if (mods.size <= N)
		res = ft_fill_c(c, mods.padding, mods.padchar, mods.align_left);
	else
	{
		len = ft_wchar_to_char(b, c);
		if (len > -1)
		{
			res = ft_fill(b, mods.padding, mods.padchar, mods.align_left);
			len = (int)((int)mods.padding > len ? mods.padding : len);
		}
	}
	if (len > -1)
		*buff = res;
	return (len);
}
