/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 00:38:00 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 03:53:36 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"

void	go_lr(t_line *line, int dir, int n)
{
	if (n == 0)
		n = 1;
	if (dir == -1)
		line->pos -= line->pos > n ? n : line->pos - 1;
	else if (dir == 1)
		line->pos += line->pos + n < line->lst_input->size ? n :
			line->lst_input->size - line->pos - 1;
	if (line->pos < 1)
		line->pos = 1;
}

void	handle_ctrl_darrow(t_line *line, int edit, int n)
{
	if (n == 0)
		n = 1;
	if (line->pos + (g_term_infos.width * n) < line->lst_input->size + !edit)
		line->pos += g_term_infos.width * n;
	else
		line->pos = line->lst_input->size;
}

void	handle_ctrl_uarrow(t_line *line, int n)
{
	if (n == 0)
		n = 1;
	if (line->pos - (g_term_infos.width * n) > 0)
		line->pos -= (g_term_infos.width * n);
	else
		line->pos = 1;
}

void	handle_darrow(t_line *line, int edit, int n)
{
	if (!edit)
		history_pn(line, HNEXT, line->input_history);
	else if (line->multi)
		handle_ctrl_darrow(line, edit, n);
}

void	handle_uarrow(t_line *line, int edit, int n)
{
	if (!edit)
		history_pn(line, HPREV, line->input_history);
	else if (line->multi)
		handle_ctrl_uarrow(line, n);
}
