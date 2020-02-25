/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstremove_if.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 20:43:46 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/24 21:32:00 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstremove_if(t_list **begin_list, void *data_ref,
	int (*cmp)(), void (*free_fct)(void *))
{
	t_list	*prev;
	t_list	*el;
	t_list	*next;

	el = *begin_list;
	prev = NULL;
	while (el)
	{
		if (cmp(el->content, data_ref) == 0)
		{
			free_fct(el->content);
			next = el->next;
			if (prev)
				prev->next = next;
			else
				*begin_list = next;
			ft_memdel((void **)&el);
			el = next;
		}
		else
		{
			prev = el;
			el = el->next;
		}
	}
}

void	ft_bilstremove_if(t_bilist **begin_list, void *data_ref,
	int (*cmp)(), void (*free_fct)(void *))
{
	t_bilist	*el;
	t_bilist	*next;

	el = *begin_list;
	while (el && ((next = el->next) || 1))
		if (cmp(el->content, data_ref) == 0)
		{
			if (el->prev)
			{
				el->prev->next = next;
				if (next)
					next->prev = el->prev;
			}
			else
			{
				*begin_list = next;
				if (next)
					next->prev = NULL;
			}
			free_fct(el->content);
			ft_memdel((void **)&el);
			el = next;
		}
		else
			el = next;
}
