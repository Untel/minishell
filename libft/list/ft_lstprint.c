/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstprint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 16:59:16 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/24 21:23:57 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	ft_lstprint(t_list *lst, char *cmt)
{
	int i;

	i = 0;
	printf("--- %d nodes ---\n", ft_lstsize(lst));
	if (cmt)
		printf("| %s\n", cmt);
	while (lst)
	{
		printf("| %d | \"%s\" (%lu)\n", i++, lst->content, lst->size);
		lst = lst->next;
	}
	printf("---------------\n\n");
}

void	ft_bilstprint(t_bilist *lst, char *cmt)
{
	int i;

	i = 0;
	printf("--- %d nodes ---\n", ft_lstsize((t_list *)lst));
	if (cmt)
		printf("| %s\n", cmt);
	while (lst)
	{
		printf("| %d | \"%s\" (%lu) (p='%s'|n='%s')\n",
			i++, lst->content, lst->size,
			lst->prev ? lst->prev->content : "",
			lst->next ? lst->next->content : "");
		lst = lst->next;
	}
	printf("---------------\n\n");
}
