/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 12:37:32 by riblanc           #+#    #+#             */
/*   Updated: 2020/03/22 12:37:33 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

t_data	*copy_at(t_data *lst, int pos)
{
	int			i;
	t_data		*cpy;
	t_lst_in	*tmp;

	if (!(cpy = malloc(sizeof(t_data))))
		return (NULL);
	init_lst(cpy);
	tmp = lst->head;
	i = 0;
	while (++i < pos)
		tmp = tmp->next;
	i = 0;
	while (tmp)
	{
		add_after(cpy, tmp->c, i);
		i++;
		tmp = tmp->next;
	}
	return (cpy);
}

char	*copy_str_at(t_data *lst, int pos)
{
	int			i;
	char		*cpy;
	t_lst_in	*tmp;

	if (!(cpy = malloc(sizeof(char) * (lst->size - pos + 1))))
		return (NULL);
	tmp = lst->end;
	i = 0;
	while (++i <= pos)
		tmp = tmp->prev;
	i = 0;
	while (tmp)
	{
		cpy[i] = tmp->c;
		i++;
		tmp = tmp->prev;
	}
	cpy[i] = 0;
	return (cpy);
}

int		del_at(t_data *lst, int pos)
{
	int			i;
	t_lst_in	*tmp;

	tmp = lst->head;
	i = 0;
	while (++i < pos)
		tmp = tmp->next;
	while (tmp)
	{
		delone(lst, i);
		i++;
		tmp = tmp->next;
	}
	return (i - pos);
}
