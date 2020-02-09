/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 12:37:52 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/09 09:12:35 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include <stdlib.h>
#include <unistd.h>

void	init_lst(t_data *lst)
{
	lst->head = NULL;
	lst->end = NULL;
	lst->size = 0;
}

int		add_empty(t_data *lst, char c)
{
	t_lst_in *new_elem;

	if (!(new_elem = malloc(sizeof(t_lst_in))))
		return (-1);
	new_elem->c = c;
	new_elem->prev = NULL;
	new_elem->next = NULL;
	lst->head = new_elem;
	lst->end = new_elem;
	lst->size++;
	return (0);
}

int		add_begin(t_data *lst, char c)
{
	t_lst_in *new_elem;

	if (!(new_elem = malloc(sizeof(t_lst_in))))
		return (-1);
	new_elem->c = c;
	new_elem->prev = NULL;
	new_elem->next = lst->head;
	lst->head->prev = new_elem;
	lst->head = new_elem;
	lst->size++;
	return (0);
}

int		add_end(t_data *lst, char c)
{
	t_lst_in *new_elem;

	if (!(new_elem = malloc(sizeof(t_lst_in))))
		return (-1);
	new_elem->c = c;
	new_elem->next = NULL;
	new_elem->prev = lst->end;
	lst->end->next = new_elem;
	lst->end = new_elem;
	lst->size++;
	return (0);
}

int		add_after(t_data *lst, char c, int pos)
{
	int			i;
	t_lst_in	*new_elem;
	t_lst_in	*tmp;

	if (!(new_elem = malloc(sizeof(t_lst_in))))
		return (-1);
	new_elem->c = c;
	tmp = lst->head;
	i = 0;
	while (++i < pos)
		tmp = tmp->next;
	new_elem->next = tmp->next;
	new_elem->prev = tmp;
	if (tmp->next == NULL)
		lst->end = new_elem;
	else
		tmp->next->prev = new_elem;
	tmp->next = new_elem;
	lst->size++;
	return (0);
}
