/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 12:36:56 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/23 19:29:17 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "line_edit.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

int		add_before(t_data *lst, char c, int pos)
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
	new_elem->next = tmp;
	new_elem->prev = tmp->prev;
	if (tmp->prev == NULL)
		lst->head = new_elem;
	else
		tmp->prev->next = new_elem;
	tmp->prev = new_elem;
	lst->size++;
	return (0);
}

void	free_all(t_data *lst)
{
	while (lst->size > 0)
		delone(lst, 1);
}

char	*convert_to_str(t_data *lst, int del)
{
	t_lst_in	*tmp;
	char		*new;
	int			i;

	tmp = lst->head ? lst->head->next : lst->head;
	if (!(new = malloc(sizeof(char) * lst->size)))
		return (NULL);
	i = 0;
	while (tmp != NULL)
	{
		new[i++] = tmp->c;
		tmp = tmp->next;
	}
	new[i] = 0;
	if (del)
	{
		free_all(lst);
		ft_memdel((void **)&lst);
	}
	return (new);
}
