/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_single_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 22:15:26 by riblanc           #+#    #+#             */
/*   Updated: 2020/05/12 23:34:53 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "line_edit.h"
#include "ft_printf.h"
#include <unistd.h>

static void	refresh_edit(t_line *line, int edit)
{
	append(&line->buf, ft_strdup("\n\x1b[s\x1b[0m\x1b[999B\r\x1b[0K"));
	if (edit)
		append(&line->buf, ft_strdup("\x1b[1m -- EDIT MODE --"));
	append(&line->buf, ft_strdup("\x1b[u\x1b[A"));
}

static void	compute_offset(int *offset, int *pos, int *buf, int *len)
{
	while ((*offset + *pos) >= g_termx)
	{
		++(*buf);
		--(*pos);
		--(*len);
	}
	while ((*offset + *len) > g_termx)
		--(*len);
}

void		refresh_single_line(t_line *line, int edit)
{
	int		pos;
	int		len;
	int		buf;
	int		offset;

	pos = line->pos;
	len = line->lst_input->size;
	buf = 0;
	offset = line->size_prompt % g_termx;
	line->buf = NULL;
	refresh_edit(line, edit);
	compute_offset(&offset, &pos, &buf, &len);
	ft_sprintf(line->seq, "\r\x1b[0m\x1b[%dC", offset);
	append(&line->buf, ft_strdup(line->seq));
	append_single_cmd(line, line->lst_input, buf, len -
			(buf > 0 && line->pos != line->lst_input->size));
	append(&line->buf, ft_strdup("\x1b[0K"));
	ft_sprintf(line->seq, "\r\x1b[%dC", pos + offset - (buf == 0));
	append(&line->buf, ft_strdup(line->seq));
	write(1, line->buf, ft_strlen(line->buf));
	ft_memdel((void **)&line->buf);
}
