/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 15:44:02 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/23 18:35:36 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "line_edit.h"
#include "ft_printf.h"
#include <stdlib.h>

extern int	g_resize;
char		*g_yank = NULL;

static void	init_select(t_select *s_sel, t_line *line, char *prompt)
{
	s_sel->sel = 0;
	if (line->pos > 1)
		line->pos = line->pos >= line->lst_input->size ?
			line->lst_input->size - 1 : line->pos;
	refresh_line(line, prompt, 1);
	line->edit_history.index = line->edit_history.len;
	s_sel->str = convert_to_str(line->lst_input, 0);
	add_history(&(line->edit_history), ft_strdup(s_sel->str),
			H_EMPTYL, line->pos);
	free(s_sel->str);
	s_sel->str = NULL;
	line->edit = 1;
}

static	int	select_handle_fp(t_select *s_sel, t_line *line, char *prompt)
{
	s_sel->lst = ft_lstindex(line->edit_history.cursor_pos,
			line->edit_history.index);
	if (s_sel->lst)
		s_sel->lst->content = (void *)((size_t)line->pos);
	if (*line->buff == 27 && handle_escape(line, prompt, 1) == 1 &&
			!(s_sel->sel = 0))
		ft_bzero(line->sel, sizeof(line->sel));
	else if (*line->buff == 'h')
		line->pos -= line->pos > 1 ? 1 : 0;
	else if (*line->buff == 'j' && line->multi)
		line->pos += (line->pos + g_termx < line->lst_input->size) ?
			g_termx : 0;
	else if (*line->buff == 'k' && line->multi)
		line->pos -= (line->pos - g_termx > 0) ? g_termx : 0;
	else if (*line->buff == 'l')
		line->pos += line->pos < line->lst_input->size ? 1 : 0;
	else if (*line->buff == 'v' && (s_sel->sel = 1))
		line->sel[0] = line->pos;
	else if (*line->buff == 'p' && !s_sel->sel)
		past(line, (line->lst_input->size == 1));
	else if (*line->buff == 'P' && !s_sel->sel)
		past(line, 1);
	else
		return (0);
	return (1);
}

static int	select_handle_sp(t_select *s_sel, t_line *line)
{
	if (*line->buff == 'w')
		go_right(line);
	else if (*line->buff == 'u')
		history_pn(line, HPREV, &(line->edit_history));
	else if (*line->buff == 0x12)
		history_pn(line, HNEXT, &(line->edit_history));
	else if (*line->buff == 'x' && !(s_sel->sel = 0))
		handle_select_x(s_sel, line, &g_yank, 0);
	else if (*line->buff == 'X' && !(s_sel->sel = 0))
		handle_select_x(s_sel, line, &g_yank, 1);
	else if (*line->buff == 10 && !s_sel->sel)
		return (-1);
	else if ((*line->buff == 'y' || *line->buff == 10) && !(s_sel->sel = 0))
	{
		g_yank = get_str_by_pos(line, 0);
		ft_bzero(line->sel, sizeof(line->sel));
	}
	else if (*line->buff == 'i' || (*line->buff == 'I' && (line->pos = 1)) ||
			(*line->buff == 'A' && (line->pos = line->lst_input->size)))
		return (-1);
	else
		return (0);
	return (1);
}

void		select_mode_lp(t_select *s_sel, t_line *line, char *prompt)
{
	if (s_sel->sel)
		line->sel[1] = line->pos < line->sel[0] ? line->pos :
			line->pos + 1;
	else
		ft_bzero(line->sel, sizeof(line->sel));
	if (line->pos > 1)
		line->pos = line->pos >= line->lst_input->size ?
			line->lst_input->size - 1 : line->pos;
	if (g_resize && !((g_resize = 0)))
		ft_printf("\r\x1b[0K%s", prompt);
	refresh_line(line, prompt, 1);
	ft_bzero(line->buff, 6);
}

void		select_mode(t_line *line, char *prompt)
{
	t_select s_sel;

	init_select(&s_sel, line, prompt);
	while (!g_sigquit && (s_sel.ret = read(0, line->buff, 1)) >= 0)
	{
		if (g_sigquit)
			break ;
		if (s_sel.ret > 0)
		{
			if (!select_handle_fp(&s_sel, line, prompt))
				if (select_handle_sp(&s_sel, line) == -1)
					break ;
			select_mode_lp(&s_sel, line, prompt);
		}
	}
	line->edit = 0;
	ft_bzero(line->sel, sizeof(line->sel));
	refresh_line(line, prompt, 0);
}
