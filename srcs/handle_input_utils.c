/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 21:44:17 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/24 22:01:20 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"
#include "list.h"
#include <unistd.h>

extern int	g_termx;

static void	go_left(t_line *line)
{
	t_lst_in	*elem;

	elem = get_elem_by_pos(line->lst_input, line->pos - 1);
	while (elem && elem->c && !ft_isalnum(elem->c))
	{
		elem = elem->prev;
		--line->pos;
	}
	while (elem && elem->c && ft_isalnum(elem->c))
	{
		elem = elem->prev;
		--line->pos;
	}
}

void		go_right(t_line *line)
{
	t_lst_in	*elem;

	elem = get_elem_by_pos(line->lst_input, line->pos);
	while (elem && elem->next && !ft_isalnum(elem->c))
	{
		elem = elem->next;
		++line->pos;
	}
	while (elem && ft_isalnum(elem->c))
	{
		elem = elem->next;
		++line->pos;
	}
	while (elem && !ft_isalnum(elem->c))
	{
		elem = elem->next;
		++line->pos;
	}
}

void		add_char(t_line *line)
{
	add_after(line->lst_input, line->buff[0], line->pos);
	++line->pos;
}

void		handle_ctrll(t_line *line, char *prompt)
{
	write(1, "\x1b[H\x1b[2J", 7);
	write(1, prompt, ft_strlen(prompt));
	refresh_line(line, prompt, 0);
}

void		handle_escape_sp(t_line *line, int edit, int *ret)
{
	if (!strcmp(line->buff, "\x1b[D"))
		line->pos -= line->pos > 1 ? 1 : 0;
	else if (!strcmp(line->buff, "\x1b[C"))
		line->pos += line->pos < line->lst_input->size ? 1 : 0;
	else if (!strcmp(line->buff, "\x1b[1;5B") && line->multi)
		line->pos += (line->pos + g_termx < line->lst_input->size + 1) ?
			g_termx : 0;
	else if (!strcmp(line->buff, "\x1b[1;5A") && line->multi)
		line->pos -= (line->pos - g_termx > 0) ? g_termx : 0;
	else if (!strcmp(line->buff, "\x1b[1;5D"))
		go_left(line);
	else if (!strcmp(line->buff, "\x1b[1;5C"))
		go_right(line);
	else if (!strcmp(line->buff, "\x1b[A") && !edit)
		history_pn(line, HPREV, &g_history);
	else if (!strcmp(line->buff, "\x1b[B") && !edit)
		history_pn(line, HNEXT, &g_history);
	else if (!strcmp(line->buff, "\x1b[H"))
		line->pos = 1;
	else if (!strcmp(line->buff, "\x1b[F"))
		line->pos = line->lst_input->size;
	else if (!strcmp(line->buff, "\x1b[3~"))
		handle_ctrld(line);
	else
		*ret = -2;
}
