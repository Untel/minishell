/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_multi_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 22:36:12 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/24 23:10:46 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "line_edit.h"

static void	refresh_edit(t_ml *ml, t_line *line, int edit)
{
	while (++ml->i <= ml->rows)
		append(&line->buf, ft_strdup("\x1b[B"));
	append(&line->buf, ft_strdup("\n\x1b[s\x1b[0m\x1b[999B\r\x1b[0K"));
	if (edit)
		append(&line->buf, ft_strdup("\x1b[1m -- EDIT MODE --"));
	append(&line->buf, ft_strdup("\x1b[u\x1b[A"));
	ml->i = ml->rpos;
	while (++ml->i <= ml->rows)
		append(&line->buf, ft_strdup("\x1b[A"));
}

static void	init_phase(t_ml *ml, t_line *line)
{
	ml->offset = line->size_prompt;
	ml->len = line->lst_input->size - 1;
	ml->rows = (ml->offset + ml->len + g_termx - 1) / g_termx;
	ml->pos = line->pos - 1;
	ml->rpos = (ml->offset + line->old_pos + g_termx) / g_termx;
	ml->old_rows = line->maxrows;
	if (ml->rows > line->maxrows)
		line->maxrows = ml->rows;
	line->buf = NULL;
	ml->i = ml->rpos;
}

static void	clear_old_rows(t_ml *ml, t_line *line, char *prompt)
{
	if (ml->old_rows - ml->rpos > 0)
	{
		ft_sprintf(line->seq, "\x1b[%dB", ml->old_rows - ml->rpos);
		append(&line->buf, ft_strdup(line->seq));
	}
	ml->i = -1;
	while (++ml->i < ml->old_rows - 1)
		append(&line->buf, ft_strdup("\r\x1b[0K\x1b[1A"));
	append(&line->buf, ft_strdup("\r\x1b[0K"));
	append(&line->buf, ft_strdup(prompt));
	append_multi_cmd(line, ml->len);
	append(&line->buf, ft_strdup("\x1b[0m"));
	if (ml->pos && ml->pos == ml->len && (ml->pos + ml->offset) % g_termx == 0)
	{
		append(&line->buf, ft_strdup("\n\r"));
		++ml->rows;
		if (ml->rows > line->maxrows)
			line->maxrows = ml->rows;
	}
}

void		refresh_multi_line(t_line *line, char *prompt, int edit)
{
	t_ml	ml;

	init_phase(&ml, line);
	refresh_edit(&ml, line, edit);
	clear_old_rows(&ml, line, prompt);
	ml.rpos2 = (ml.offset + ml.pos + g_termx) / g_termx;
	if (ml.rows - ml.rpos2 > 0)
	{
		ft_sprintf(line->seq, "\x1b[%dA", ml.rows - ml.rpos2);
		append(&line->buf, ft_strdup(line->seq));
	}
	ml.col = (ml.offset + ml.pos) % g_termx;
	if (ml.col)
		ft_sprintf(line->seq, "\r\x1b[%dC", ml.col);
	else
		ft_sprintf(line->seq, "\r");
	append(&line->buf, ft_strdup(line->seq));
	line->old_pos = ml.pos;
	write(1, line->buf, ft_strlen(line->buf));
	ft_memdel((void **)&line->buf);
}
