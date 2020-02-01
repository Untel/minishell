/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 15:50:17 by adda-sil          #+#    #+#             */
/*   Updated: 2019/11/05 18:23:09 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstnew(void *content, size_t size)
{
	t_list *node;

	if (!(node = malloc(sizeof(t_list))))
		return (NULL);
	node->content = content;
	node->size = size;
	node->next = NULL;
	return (node);
}
