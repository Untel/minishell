/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 16:24:32 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 01:00:13 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"
#include <stdlib.h>

void		ft_swap(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

static char	*fill_str(t_line *line, int del, int i[3])
{
	t_lst_in	*tmp;
	char		*str;

	tmp = get_elem_by_pos(line->lst_input, i[0] == 0 ? line->pos : i[0]);
	if (!(str = malloc(sizeof(char) * (i[1] - i[0] + 2))))
		return (NULL);
	str[i[1] - i[0] + 1] = 0;
	i[2] = 0;
	while (tmp)
	{
		if (i[2] <= i[1] - i[0])
			str[i[2]++] = tmp->c;
		tmp = tmp->next;
	}
	if (del)
	{
		i[2] = -1;
		while (++i[2] <= i[1] - i[0])
			delone(line->lst_input, (i[0] == 0 ? line->pos : i[0]) + 1);
		while (line->pos > i[0] && i[0] != 0 && i[1] != 0)
			--line->pos;
	}
	return (str);
}

char		*get_str_by_pos(t_line *line, int del)
{
	char		*str;
	int			i[3];

	i[0] = line->sel[0];
	i[1] = line->sel[1] - (line->sel[1] > i[0]);
	if (i[0] >= i[1])
		ft_swap(i, i + 1);
	i[1] -= line->pos >= line->lst_input->size || i[1] >= line->lst_input->size;
	if (line->copy_buff)
		free(line->copy_buff);
	str = fill_str(line, del, i);
	return ((line->copy_buff = str));
}

void		past(t_line *line, int before)
{
	int		i;
	int		len;
	char	*str;

	if (line->copy_buff)
	{
		++line->edit_history.index;
		len = ft_strlen(line->copy_buff);
		i = -1;
		while (++i < len)
		{
			++line->pos;
			add_after(line->lst_input, line->copy_buff[i], line->pos - before);
		}
		str = convert_to_str(line->lst_input, 0);
		free_history(&line->edit_history, line->edit_history.index);
		add_history(&(line->edit_history), ft_strdup(str), H_EMPTYL, line->pos);
		free(str);
	}
}

void		handle_select_x(t_select *s_sel, t_line *line, char **yank, int maj)
{
	++line->edit_history.index;
	if (!maj)
		*yank = get_str_by_pos(line, 1);
	ft_bzero(line->sel, sizeof(line->sel));
	if (maj)
		handle_ctrlu(line);
	s_sel->str = convert_to_str(line->lst_input, 0);
	free_history(&line->edit_history, line->edit_history.index);
	add_history(&(line->edit_history), ft_strdup(s_sel->str),
			H_EMPTYL, line->pos);
	free(s_sel->str);
	s_sel->str = NULL;
}
