/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_single_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 22:15:26 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 01:28:50 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "line_edit.h"
#include "ft_printf.h"
#include <unistd.h>

static void	refresh_edit(t_line *line, int edit)
{
	append(&line->buf, "\n\x1b[s\x1b[0m\x1b[999B\r\x1b[0K");
	if (edit)
		append(&line->buf, "\x1b[1m -- EDIT MODE --");
	append(&line->buf, "\x1b[u\x1b[A");
}

static void	compute_offset(int *offset, int *pos, int *buf, int *len)
{
	while ((*offset + *pos) >= g_term_size.width)
	{
		++(*buf);
		--(*pos);
		--(*len);
	}
	while ((*offset + *len) > g_term_size.width)
		--(*len);
}

void		refresh_single_line(t_line *line, int edit)
{
	int		pos;
	int		len;
	int		input_len;
	int		buf;
	int		offset;

	pos = line->pos;
	len = line->lst_input->size +
		(line->complete.str ? ft_strlen(line->complete.str) : 0);
	input_len = len;
	buf = 0;
	offset = line->size_prompt % g_term_size.width;
	line->buf = NULL;
	refresh_edit(line, edit);
	compute_offset(&offset, &pos, &buf, &len);
	ft_sprintf(line->seq, "\r\x1b[0m\x1b[%dC", offset);
	append(&line->buf, (line->seq));
	append_single_cmd(line, line->lst_input, buf, len -
			(buf > 0 && line->pos != input_len));
	append(&line->buf, "\x1b[0K");
	ft_sprintf(line->seq, "\r\x1b[%dC", pos + offset - (buf == 0));
	append(&line->buf, (line->seq));
	write(1, line->buf, ft_strlen(line->buf));
	ft_memdel((void **)&line->buf);
}
