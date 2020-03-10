/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:34:51 by riblanc           #+#    #+#             */
/*   Updated: 2020/03/10 20:32:43 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"
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
		ft_fprintf(2, "adding %c\n", tmp->c);
		add_after(cpy, tmp->c, i);
		i++;
		tmp = tmp->next;
	}
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
