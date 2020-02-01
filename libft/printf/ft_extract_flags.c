/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extract_flags.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/29 20:17:59 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/14 01:48:30 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

static size_t
	ft_handle_size(const char *str, t_mod *mods)
{
	size_t ret;

	ret = 1;
	if (ft_is_conv("lL", *str))
	{
		if (ft_is_conv("lL", *(str + 1)) && ret++)
			mods->size = LL;
		else
			mods->size = L;
	}
	else
	{
		if (ft_is_conv("hH", *(str + 1)) && ret++)
			mods->size = HH;
		else
			mods->size = H;
	}
	return (ret);
}

static size_t
	ft_handle_jusitfy_side(t_mod *mods)
{
	mods->align_left = 1;
	mods->padchar = ' ';
	return (1);
}

static size_t
	ft_handle_padding_size(const char *str, t_mod *mods, va_list args)
{
	int s;

	if (*str == '*')
	{
		s = va_arg(args, int);
		if (s >= 0)
			mods->padding = (size_t)s;
		else
		{
			ft_handle_jusitfy_side(mods);
			mods->padding = (size_t)(s * -1);
		}
		return (1);
	}
	mods->padding = ft_atoi(str);
	return (ft_intlen(mods->padding));
}

static size_t
	ft_handle_precision_size(const char *str, t_mod *mods, va_list args)
{
	char *ptr;

	ptr = (char *)str;
	str++;
	if (*str == '*')
	{
		mods->precision = (int)va_arg(args, int);
		if (mods->precision < 0)
			mods->precision = -1;
		return (2);
	}
	if (!ft_isdigit(*str))
	{
		mods->precision = 0;
		return (str - ptr);
	}
	mods->precision = ft_atoi(str);
	while (ft_isdigit(*str))
		str++;
	return ((str - ptr));
}

int
	ft_extract_flags(const char *str, va_list args, t_mod *mods)
{
	int			i;

	i = 0;
	while (str[i])
		if (str[i] == '0')
			(mods->padchar = '0') && i++;
		else if (ft_isdigit(str[i]) || str[i] == '*')
			i += ft_handle_padding_size(&str[i], mods, args);
		else if (str[i] == '.')
			i += ft_handle_precision_size(&str[i], mods, args);
		else if (str[i] == '-')
			i += ft_handle_jusitfy_side(mods);
		else if (str[i] == '+' || str[i] == ' ')
			mods->sign = str[i++];
		else if (str[i] == '#')
			(mods->alt = 1) && i++;
		else if (ft_is_conv("lLhH", str[i]))
			i += ft_handle_size(&str[i], mods);
		else if (str[i] == '\'')
			(mods->sep = ',') && i++;
		else
			break ;
	mods->conv = str[i];
	return (i);
}
