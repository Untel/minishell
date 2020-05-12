/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 00:38:00 by riblanc           #+#    #+#             */
/*   Updated: 2020/05/13 00:41:51 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"

void	handle_ctrl_darrow(t_line *line, int edit)
{
	if (line->pos + g_termx < line->lst_input->size + !edit)
		line->pos += g_termx;
	else
		line->pos = line->lst_input->size;
}

void	handle_ctrl_uarrow(t_line *line)
{
	if (line->pos - g_termx > 0)
		line->pos -= g_termx;
	else
		line->pos = 1;
}

void	handle_darrow(t_line *line, int edit)
{
	if (!edit)
		history_pn(line, HNEXT, &g_history);
	else
		handle_ctrl_darrow(line, edit);
}

void	handle_uarrow(t_line *line, int edit)
{
	if (!edit)
		history_pn(line, HPREV, &g_history);
	else
		handle_ctrl_uarrow(line);
}
