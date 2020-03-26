/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 12:36:56 by riblanc           #+#    #+#             */
/*   Updated: 2020/03/26 10:53:13 by riblanc          ###   ########.fr       */
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

void	ft_swap(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void	append_multi_cmd(t_line *line, int max)
{
	int		sel[2];
	char	*str;
	char	*part[3];
	int		sens;

	sel[0] = line->sel[0];
	sel[1] = line->sel[1];
	sens = (sel[0] > sel[1]);
	if (sel[0] > sel[1] && (++sel[0] || 1))
		ft_swap(&sel[0], &sel[1]);
	str = convert_to_str(line->lst_input, 0);
	if (sel[0] != sel[1])
	{
		part[0] = ft_substr(str, 0, sel[0] - !sens);
		part[1] = ft_substr(str, sel[0] - !sens, sel[1] - sel[0] - 1);
		part[2] = ft_substr(str, sel[1] - 1 - !sens, max - sel[1] + 1 + !sens);
		append(&line->buf, part[0]);
		append(&line->buf, ft_strdup("\x1b[7m"));
		append(&line->buf, part[1]);
		append(&line->buf, ft_strdup("\x1b[0m"));
		append(&line->buf, part[2]);
	}
	else
		append(&line->buf, ft_strdup(str));
	append(&line->buf, ft_strdup("\x1b[0K"));
	ft_memdel((void **)&str);
}

void	append_single_cmd(t_line *line, t_data *lst, int offset, int max)
{
	t_lst_in	*tmp;
	int			i;
	int			a;
	int			b;
	int			sens;

	a = line->sel[0];
	b = line->sel[1];
	sens = (a > b);
	if (a > b && (++a || 1))
		ft_swap(&a, &b);
	tmp = lst->head;
	i = -1;
	while (++i < offset && tmp)
		tmp = tmp->next;
	while (--max >= 0 && tmp != NULL)
	{
		if (i >= a + sens && a != b)
			append(&line->buf, ft_strdup("\x1b[7m"));
		if (i++ >= b - !sens && a != b)
			append(&line->buf, ft_strdup("\x1b[0m"));
		line->seq[0] = tmp->c;
		line->seq[1] = 0;
		append(&line->buf, ft_strdup(line->seq));
		tmp = tmp->next;
	}
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
