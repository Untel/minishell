/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/23 19:27:42 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/23 19:28:57 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"

static void	ft_swap(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void		append_multi_cmd(t_line *line, int max)
{
	int		sel[2];
	char	*s[4];
	int		sens;

	sel[0] = line->sel[0];
	sel[1] = line->sel[1];
	sens = (sel[0] > sel[1]);
	if (sel[0] > sel[1] && (++sel[0] || 1))
		ft_swap(&sel[0], &sel[1]);
	s[3] = convert_to_str(line->lst_input, 0);
	if (sel[0] != sel[1])
	{
		s[0] = ft_substr(s[3], 0, sel[0] - !sens);
		s[1] = ft_substr(s[3], sel[0] - !sens, sel[1] - sel[0] - 1);
		s[2] = ft_substr(s[3], sel[1] - 1 - !sens, max - sel[1] + 1 + !sens);
		append(&line->buf, s[0]);
		append(&line->buf, ft_strdup("\x1b[7m"));
		append(&line->buf, s[1]);
		append(&line->buf, ft_strdup("\x1b[0m"));
		append(&line->buf, s[2]);
	}
	else
		append(&line->buf, ft_strdup(s[3]));
	append(&line->buf, ft_strdup("\x1b[0K"));
	ft_memdel((void **)&s[3]);
}

void		append_single_cmd(t_line *line, t_data *lst, int offset, int max)
{
	t_lst_in	*tmp;
	int			i[3];
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
	while (--max >= 0 && tmp != NULL)
	{
		if (i[2] >= *i + sens && *i != i[1])
			append(&line->buf, ft_strdup("\x1b[7m"));
		if (i[2]++ >= i[1] - !sens && *i != i[1])
			append(&line->buf, ft_strdup("\x1b[0m"));
		line->seq[0] = tmp->c;
		line->seq[1] = 0;
		append(&line->buf, ft_strdup(line->seq));
		tmp = tmp->next;
	}
}
