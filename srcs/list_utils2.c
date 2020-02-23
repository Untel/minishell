/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:11:01 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/23 17:45:37 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
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

void	affiche_inv(t_data *lst)
{
	t_lst_in *tmp;

	tmp = lst->end;
	while (tmp != NULL)
	{
		write(1, &tmp->c, 1);
		tmp = tmp->prev;
	}
}

char	*convert_to_str(t_data *lst)
{
	t_lst_in	*tmp;
	char		*new;
	int			i;

	tmp = lst->end;
	if (!(new = malloc(sizeof(char) * (lst->size + 1))))
		return (NULL);
	i = 0;
	while (tmp != NULL)
	{
		new[i++] = tmp->c;
		tmp = tmp->prev;
	}
	free_all(lst);
	ft_memdel((void **)&lst);
	return (new);
}
