/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 16:15:50 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/24 18:24:37 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	if (alst && new)
	{
		if (*alst)
			ft_lstlast(*alst)->next = new;
		else
			*alst = new;
	}
}

void	ft_bilstadd_back(t_bilist **alst, t_bilist *new)
{
	t_bilist	*el;

	if (alst && new)
	{
		if (*alst)
		{
			el = (t_bilist *)ft_lstlast((t_list *)*alst);
			el->next = new;
			new->prev = el;
		}
		else
			*alst = new;
	}
}
