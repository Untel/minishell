/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 18:08:46 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/25 18:08:52 by adda-sil         ###   ########.fr       */
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
