/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstshift.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 15:54:26 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/19 13:24:13 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstshift(t_list **alst)
{
	t_list *el;

	if (alst)
	{
		el = *alst;
		*alst = (*alst)->next;
		return (el);
	}
	return (NULL);
}
