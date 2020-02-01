/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 17:02:01 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/05 18:28:06 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *), void (*del)(t_list *))
{
	t_list *first;
	t_list *node;

	if (!lst || !(first = f(lst)))
		return (NULL);
	node = first;
	while ((lst = lst->next))
	{
		if (!(node->next = f(lst)))
		{
			ft_lstclear(&first, del);
			return (NULL);
		}
		node = node->next;
	}
	node->next = NULL;
	return (first);
}
