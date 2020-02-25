/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 16:13:02 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/24 18:22:50 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (lst)
		while (lst->next)
			lst = lst->next;
	return (lst);
}

t_list	*ft_lstlast_if(t_list *lst, int (*fn)(t_list *prev, t_list *el))
{
	t_list *last;
	t_list *prev;

	prev = NULL;
	last = NULL;
	while (lst)
	{
		if (fn(prev, lst))
			last = lst;
		prev = lst;
		lst = lst->next;
	}
	return (last);
}
