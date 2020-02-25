/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast copy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 16:13:02 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/24 16:06:40 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstpop(t_list **lst)
{
	t_list *last;
	t_list *prev;

	prev = NULL;
	last = NULL;
	if (!lst)
		return (NULL);
	last = *lst;
	while (last)
	{
		if (!last->next)
		{
			if (prev)
				prev->next = NULL;
			return (last);
		}
		prev = last;
		last = last->next;
	}
	return (last);
}
