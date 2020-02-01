/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/30 11:39:54 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/14 02:16:26 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

static size_t
	ft_lst_size(t_list *lst)
{
	size_t	len;
	t_list	*el;

	el = lst;
	len = 0;
	while (el)
	{
		len += el->size;
		el = el->next;
	}
	return (len);
}

int
	ft_convert(t_mod mods, t_list **lst, va_list args)
{
	char	*buff;
	int		len;

	len = 0;
	buff = NULL;
	if (ft_is_conv("dDiIuU", mods.conv))
		len = ft_parse_int(&buff, mods, args);
	else if (ft_is_conv("sS", mods.conv))
		len = ft_parse_string(&buff, mods, args);
	else if (ft_is_conv("xXpPpoO", mods.conv))
		len = ft_parse_base(&buff, mods, args);
	else if (ft_is_conv("fFgGeE", mods.conv))
		len = ft_parse_float(&buff, mods, args);
	else if (ft_is_conv("nN", mods.conv))
		*(va_arg(args, int *)) = ft_lst_size(*lst);
	else
		len = ft_parse_char(&buff, mods, args);
	if (len > -1 && buff)
		ft_lstadd_back(lst, ft_lstnew(buff, len));
	return (len);
}

static int
	ft_addstr(const char *str, t_list **lst, int len)
{
	char	*ptr;
	t_list	*el;

	if (!(ptr = ft_substr(str, 0, len)))
		return (0);
	if (!(el = ft_lstnew((void *)ptr, len)))
	{
		free(ptr);
		return (0);
	}
	ft_lstadd_back(lst, el);
	return (1);
}

int
	ft_format(const char *str, t_list **lst, va_list args)
{
	t_mod		mods;
	int			i;

	mods = (t_mod) {
		.padding = 0, .padchar = ' ', .sign = 0,
		.align_left = 0, .precision = -1, .alt = 0, .size = N,
		.sep = '\0', .trail = 1, .conv = 0
	};
	i = ft_extract_flags(str, args, &mods);
	if (i > -1 && !str[i])
		return (i);
	if (i == -1 || ft_convert(mods, lst, args) == -1)
		return (-1);
	return (i + 1);
}

int
	ft_split_to_list(const char *str, t_list **lst, va_list args)
{
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	len = ft_strlen(str);
	while (i < len)
		if (str[i] == '%')
		{
			if (i - j > 0 && !ft_addstr(&str[j], lst, (i - j)))
				return (0);
			if (str[i + 1] && (j = ft_format(&str[i + 1], lst, args)) > -1)
				i += j + 1;
			else if (j == -1)
				return (0);
			else
				i++;
			j = i;
		}
		else
			i++;
	return (str[j] ? ft_addstr(&str[j], lst, (i - j)) : 1);
}
