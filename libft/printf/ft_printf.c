/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 20:20:55 by adda-sil          #+#    #+#             */
/*   Updated: 2020/01/11 19:28:02 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

static void
	ft_clearfn(t_list *lst)
{
	if (lst->content)
		free(lst->content);
}

int
	ft_printf(const char *str, ...)
{
	t_list	*el;
	t_list	*fel;
	va_list	args;
	int		len;

	va_start(args, str);
	len = 0;
	fel = NULL;
	if (ft_split_to_list(str, &fel, args) && ((el = fel) || 1))
		while (el)
		{
			len += el->size;
			write(OUTPUT_FD, (char *)el->content, el->size);
			el = el->next;
		}
	else
		len = -1;
	ft_lstclear(&fel, ft_clearfn);
	va_end(args);
	return (len);
}

int
	ft_fprintf(int fd, const char *str, ...)
{
	t_list	*el;
	t_list	*fel;
	int		len;
	va_list	args;

	va_start(args, str);
	len = 0;
	fel = NULL;
	if (ft_split_to_list(str, &fel, args) && ((el = fel) || 1))
		while (el)
		{
			len += el->size;
			write(fd, (char *)el->content, el->size);
			el = el->next;
		}
	else
		len = -1;
	ft_lstclear(&fel, ft_clearfn);
	va_end(args);
	return (len);
}

int
	ft_sprintf(char *buff, const char *str, ...)
{
	t_list	*el;
	t_list	*fel;
	size_t	i;
	va_list	args;
	int		err;

	va_start(args, str);
	i = 0;
	fel = NULL;
	err = 0;
	if (ft_split_to_list(str, &fel, args) && (el = fel))
		while (el)
		{
			str = (char *)el->content;
			while (el->size-- > 0)
				buff[i++] = *str++;
			el = el->next;
		}
	else
		err = 1;
	ft_lstclear(&fel, ft_clearfn);
	buff[i] = 0;
	va_end(args);
	return (err ? -1 : i);
}
