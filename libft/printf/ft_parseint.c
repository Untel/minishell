/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parseint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/03 16:32:41 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/14 02:02:08 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

int64_t
	ft_get_sized_int(va_list args, t_mod mods)
{
	int64_t val;

	if (mods.size == HH)
		val = (int64_t)(char)va_arg(args, int);
	else if (mods.size == H)
		val = (int64_t)(short int)va_arg(args, int);
	else if (mods.size == N)
		val = (int64_t)va_arg(args, int);
	else if (mods.size == L)
		val = (int64_t)va_arg(args, long int);
	else if (mods.size == LL)
		val = (int64_t)va_arg(args, int64_t);
	else
		val = 0;
	return (val);
}

uint64_t
	ft_get_sized_uint(va_list args, t_mod mods)
{
	uint64_t val;

	if (mods.size == HH)
		val = (uint64_t)(unsigned char)va_arg(args, int);
	else if (mods.size == H)
		val = (uint64_t)(unsigned short int)va_arg(args, int);
	else if (mods.size == N)
		val = (uint64_t)va_arg(args, unsigned int);
	else if (mods.size == L)
		val = (uint64_t)va_arg(args, unsigned long int);
	else if (mods.size == LL)
		val = (uint64_t)va_arg(args, uint64_t);
	else
		val = 0;
	return (val);
}

char
	*ft_apply_int_precision(char *res, t_mod mods)
{
	if (res && ft_strlen(res) == 1 && *res == '0')
		res = ft_f(res, ft_strdup(""));
	if (res && mods.precision > 0)
		res = ft_f(res, ft_fill(res, mods.precision, '0', 0));
	if (res && mods.sign)
		res = ft_f(res, ft_add_sign(mods.sign, res));
	if (res)
		res = ft_f(res,
			ft_fill(res, mods.padding, ' ', mods.align_left));
	return (res);
}

char
	*ft_apply_int_flags(char *res, t_mod mods)
{
	if (!(res = ft_strdup(res)))
		return (NULL);
	if (res && mods.precision != -1)
		return (ft_apply_int_precision(res, mods));
	else if (res)
	{
		if (mods.sign && mods.padchar == ' ')
			res = ft_f(res, ft_add_sign(mods.sign, res));
		else if (mods.sign && mods.padding > 0)
			mods.padding--;
		if (!res || !(res = ft_f(res,
			ft_fill(res, mods.padding, mods.padchar, mods.align_left))))
			return (NULL);
		if (mods.sign && mods.padchar == '0')
			res = ft_f(res, ft_add_sign(mods.sign, res));
	}
	return (res);
}

int
	ft_parse_int(char **buff, t_mod mods, va_list args)
{
	char	*res;
	int64_t li;

	if (!ft_is_conv("uU", mods.conv))
	{
		li = ft_get_sized_int(args, mods);
		res = ft_itoa_wrapper((uint64_t)(li < 0 ? -li : li), mods.sep, li < 0);
	}
	else
		res = ft_itoa_wrapper(ft_get_sized_uint(args, mods), mods.sep, 0);
	if (res && *res == '-' && (res = ft_f(res, ft_strdup(res + 1))))
		mods.sign = '-';
	if (!res || !(res = ft_f(res, ft_apply_int_flags(res, mods))))
		return (-1);
	*buff = res;
	return (ft_strlen(*buff));
}
