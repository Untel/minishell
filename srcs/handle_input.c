/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 11:04:29 by riblanc           #+#    #+#             */
/*   Updated: 2020/05/13 16:33:47 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "list.h"
#include "line_edit.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <termios.h>

static void	handle_backspace(t_line *line)
{
	if (line->pos > 1)
		delone(line->lst_input, line->pos--);
	return ;
}

int			handle_ctrld(t_line *line)
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

	ret = -1;
	if ((ret = read(0, line->buff + 1, 5)) > 0)
		handle_escape_sp(line, edit, &ret);
	if (!ret || ret == -1)
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

int			handle_input(t_line *line, char *prompt)
{
	if (line->buff[0] == 9)
		auto_complete(line);
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
	else if (line->buff[0] == 10 || line->buff[0] == 3)
		return (1 + (line->buff[0] == 3));
	else if (ft_isprint(line->buff[0]))
		add_char(line);
	return (0);
}
