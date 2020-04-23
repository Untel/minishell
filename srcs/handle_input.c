/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 11:04:29 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/23 16:12:34 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "list.h"
#include "line_edit.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <termios.h>

extern int	g_termx;

static void	handle_backspace(t_line *line)
{
	if (line->pos > 1)
		delone(line->lst_input, line->pos--);
	return ;
}

void		go_right(t_line *line)
{ t_lst_in	*elem;

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

static int	handle_ctrld(t_line *line)
{
	if (line->lst_input->size == 1)
		return (-1);
	else if (line->lst_input->size > 1
			&& line->pos + 1 <= line->lst_input->size)
		delone(line->lst_input, line->pos + 1);
	return (0);
}

int			handle_escape(t_line *line, char *prompt, int edit)
{
	int		ret;

	ret = -2;
	if (!g_sigquit && ((ret = read(0, line->buff + 1, 5)) > 0))
	{
		if (!strcmp(line->buff, "\x1b[D"))
			line->pos -= line->pos > 1 ? 1 : 0;
		else if (!strcmp(line->buff, "\x1b[C"))
			line->pos += line->pos < line->lst_input->size ? 1 : 0;
		else if (!strcmp(line->buff, "\x1b[1;5B") && line->multi)
			line->pos += (line->pos + g_termx < line->lst_input->size + 1) ? g_termx : 0;
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
			ret = -2;
	}
	if (!ret || ret == -2)
	{
		if (!edit)
			select_mode(line, prompt);
		else
			return (1);
	}
	return (0);
}

void		handle_ctrlu(t_line *line)
{
	while (line->lst_input->size > 0)
	{
		delone(line->lst_input, 1);
		line->pos -= line->pos > 0 ? 1 : 0;
	}
	add_empty(line->lst_input, 0);
	++line->pos;
}

static void	add_char(t_line *line)
{
	add_after(line->lst_input, line->buff[0], line->pos);
	++line->pos;
}

static void	handle_ctrll(t_line *line, char *prompt)
{
	write(1, "\x1b[H\x1b[2J", 7);
	write(1, prompt, ft_strlen(prompt));
	refresh_line(line, prompt, 0);
}

int			handle_input(t_line *line, char *prompt)
{
	if (line->buff[0] == 4)
		return (handle_ctrld(line));
	else if (line->buff[0] == 12)
		handle_ctrll(line, prompt);
	if (line->buff[0] == 127 || line->buff[0] == 8)
		handle_backspace(line);
	else if (line->buff[0] == 27)
		handle_escape(line, prompt, 0);
	else if (line->buff[0] == 21)
		handle_ctrlu(line);
	else if (line->buff[0] == 10)
		return (1);
	else if (ft_isprint(line->buff[0]))
		add_char(line);
	return (0);
}
