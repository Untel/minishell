/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_before.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 15:09:54 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/11 15:24:20 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_lstadd_before(t_list **lst, t_list *before_el, t_list *new_el)
{
	t_list *iter;
	t_list *prev;

	iter = *lst;
	prev = NULL;
	if (!before_el || !*lst)
		return (ft_lstadd_front(lst, new_el));
	while (iter)
	{
		if (iter == before_el)
		{
			if (prev == NULL)
				return (ft_lstadd_front(lst, new_el));
			else
			{
				prev->next = new_el;
				new_el->next = iter;
			}
			return ;
		}
		prev = iter;
		iter = iter->next;
	}
}
