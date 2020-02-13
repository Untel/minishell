/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:34:51 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/13 07:15:42 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include <stdlib.h>
#include <unistd.h>

static void	first_case(t_lst_in **del_elem, t_data *lst)
{
	*del_elem = lst->head;
	if (!(lst->head = lst->head->next))
		lst->end = NULL;
	else
		lst->head->prev = NULL;
}

static void	scd_case(t_lst_in **del_elem, t_data *lst)
{
	*del_elem = lst->end;
	lst->end->prev->next = NULL;
	lst->end = lst->end->prev;
}

static void	thd_case(t_lst_in **del_elem, t_data *lst, int pos)
{
	t_lst_in	*tmp;
	int			i;

	tmp = lst->head;
	i = 0;
	while (++i < pos)
		tmp = tmp->next;
	*del_elem = tmp;
	tmp->prev->next = tmp->next;
	tmp->next->prev = tmp->prev;
}

int			delone(t_data *lst, int pos)
{
	t_lst_in	*del_elem;

	if (lst->size == 0)
		return (-1);
	if (pos == 1)
		first_case(&del_elem, lst);
	else if (pos == lst->size)
		scd_case(&del_elem, lst);
	else
		thd_case(&del_elem, lst, pos);
	free(del_elem);
	--lst->size;
	return (0);
}

t_lst_in	*get_elem_by_pos(t_data *lst, int pos)
{
	t_lst_in	*tmp;
	int			i;

	i = -1;
	tmp = lst->head;
	while (++i < pos && tmp)
		tmp = tmp->next;
	return (tmp);
}
