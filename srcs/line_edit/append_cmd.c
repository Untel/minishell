/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 19:27:42 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 01:18:27 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"

void
	append_multi_cmd_2(t_line *line, char *s[4])
{
	if (line->complete.pos != -1 && line->complete.str &&
			*line->complete.str)
	{
		s[0] = ft_substr(s[3], 0, line->complete.pos - 1);
		s[1] = ft_substr(s[3], line->complete.pos - 1, ft_strlen(s[3]) -
				line->complete.pos + 1);
		append(&line->buf, s[0]);
		ft_memdel((void **)s);
		append(&line->buf, "\x1b[33m");
		append(&line->buf, line->complete.str);
		append(&line->buf, "\x1b[0m");
		append(&line->buf, s[1]);
		ft_memdel((void **)(s + 1));
	}
	else
	{
		append(&line->buf, s[3]);
		ft_memdel((void **)(s + 3));
	}
}

static void
	append_multi_cmd_core(t_line *line, int sel[2], char *s[4], int max)
{
	int sens;

	sens = line->sel[0] > line->sel[1];
	s[0] = ft_substr(s[3], 0, sel[0] - !sens);
	s[1] = ft_substr(s[3], sel[0] - !sens, sel[1] - sel[0] - 1);
	s[2] = ft_substr(s[3], sel[1] - 1 - !sens, max - sel[1] + 1 + !sens);
	append(&line->buf, s[0]);
	append(&line->buf, "\x1b[7m");
	append(&line->buf, s[1]);
	append(&line->buf, "\x1b[0m");
	append(&line->buf, s[2]);
	ft_memdel((void **)s);
	ft_memdel((void **)s + 1);
	ft_memdel((void **)s + 2);
}

void
	append_multi_cmd(t_line *line, int max)
{
	int		sel[2];
	char	*s[4];

	sel[0] = line->sel[0];
	sel[1] = line->sel[1];
	if (sel[0] > sel[1] && (++sel[0] || 1))
		ft_swap(&sel[0], &sel[1]);
	s[3] = convert_to_str(line->lst_input, 0);
	if (sel[0] != sel[1])
		append_multi_cmd_core(line, sel, s, max);
	else
		append_multi_cmd_2(line, s);
	append(&line->buf, "\x1b[0K");
	ft_memdel((void **)s + 3);
}

void
	append_single_cmd_2(t_line *line, int *max, int i[5])
{
	append(&line->buf, line->seq);
	if (++i[3] == line->complete.pos)
	{
		append(&line->buf, "\x1b[33m");
		i[4] = -1;
		while (line->complete.str[++i[4]])
		{
			line->seq[0] = line->complete.str[i[4]];
			line->seq[1] = 0;
			append(&line->buf, line->seq);
			*max -= 1;
		}
		append(&line->buf, "\x1b[0m");
	}
}

void
	append_single_cmd(t_line *line, t_data *lst, int offset, int max)
{
	t_lst_in	*tmp;
	int			i[5];
	int			sens;

	*i = line->sel[0];
	i[1] = line->sel[1];
	sens = (*i > i[1]);
	if (*i > i[1] && (++i[0] || 1))
		ft_swap(i, i + 1);
	tmp = lst->head;
	i[2] = -1;
	while (++i[2] < offset && tmp)
		tmp = tmp->next;
	i[3] = 0;
	while (--max >= 0 && tmp != NULL)
	{
		if (i[2] >= *i + sens && *i != i[1])
			append(&line->buf, "\x1b[7m");
		if (i[2]++ >= i[1] - !sens && *i != i[1])
			append(&line->buf, "\x1b[0m");
		line->seq[0] = tmp->c;
		line->seq[1] = 0;
		append_single_cmd_2(line, &max, i);
		tmp = tmp->next;
	}
}
