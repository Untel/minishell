/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 01:44:57 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/23 15:31:47 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"
#include "ft_printf.h"
#include <stdlib.h>

void	print_history(t_history *history)
{
	t_list *head;

	head = history->lst;
	ft_fprintf(2, "HISTORY:\n");
	ft_fprintf(2, "len: %d\n", history->len);
	while (head)
	{
		ft_fprintf(2, "[%s]\n", (char *)head->content);
		head = head->next;
	}
}

void	free_history(t_history *history, int index)
{
	t_list	*tmp;
	t_list	**lst;
	t_list	**pos;

	lst = &history->lst;
	pos = &history->cursor_pos;
	while (--index >= 0 && *lst && *pos)
	{
		pos = &(*pos)->next;
		lst = &(*lst)->next;
	}
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_memdel((void **)&(*lst)->content);
		free(*lst);
		*lst = tmp;
		--history->len;
	}
	while (*pos)
	{
		tmp = (*pos)->next;
		free(*pos);
		*pos = tmp;
	}
}
