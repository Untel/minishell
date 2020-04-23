/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 11:04:29 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/23 15:20:45 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "list.h"
#include "line_edit.h"
#include "ft_printf.h"
#include <stdlib.h>
#include <termios.h>

char		*g_yank = NULL;
extern int	g_resize;
extern int	g_termx;

static void	handle_backspace(t_line *line)
{
	if (line->pos > 1)
		delone(line->lst_input, line->pos--);
	return ;
}

static void	go_right(t_line *line)
{
	t_lst_in	*elem;

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

static void	select_mode(t_line *line, char *prompt);

static int	handle_escape(t_line *line, char *prompt, int edit)
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

static void	ft_swap(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

static char *get_str_by_pos(t_line *line, int del)
{
	t_lst_in	*tmp;
	char		*str;
	int			a;
	int			b;
	int			i;

	a = line->sel[0];
	b = line->sel[1] - (line->sel[1] > a);
	if (a >= b)
		ft_swap(&a, &b);
	b -= (line->pos >= line->lst_input->size || b >= line->lst_input->size);
	if (line->copy_buff)
		free(line->copy_buff);
	tmp = get_elem_by_pos(line->lst_input, a == 0 ? line->pos : a);
	if (!(str = malloc(sizeof(char) * (b - a + 2))))
		return (NULL);
	str[b - a + 1] = 0;
	i = 0;
	while (tmp)
	{
		if (i <= b - a)
			str[i++] = tmp->c;
		tmp = tmp->next;
	}
	if (del)
	{
		i = -1;
		while (++i <= b - a)
			delone(line->lst_input, (a == 0 ? line->pos : a) + 1);
		while (line->pos > a && a != 0 && b != 0)
			--line->pos;
	}
	return ((line->copy_buff = str));
}

static void	past(t_line *line, int before)
{
	int		i;
	int		len;
	char	*str;

	if (line->copy_buff)
	{

		++line->edit_history.index;
		len = ft_strlen(line->copy_buff);
		i = -1;
		while (++i < len)
		{
			++line->pos;
			add_after(line->lst_input, line->copy_buff[i], line->pos - before);
		}
		str = convert_to_str(line->lst_input, 0);
		free_history(&line->edit_history, line->edit_history.index);
		add_history(&(line->edit_history), ft_strdup(str), H_EMPTYL, line->pos);
		free (str);
	}
}

static void	handle_ctrlu(t_line *line)
{
	while (line->lst_input->size > 0)
	{
		delone(line->lst_input, 1);
		line->pos -= line->pos > 0 ? 1 : 0;
	}
	add_empty(line->lst_input, 0);
	++line->pos;
}

static void	select_mode(t_line *line, char *prompt)
{
	int		ret;
	int		sel;
	char	*str;
	t_list	*lst;
	sel = 0;
	if (line->pos > 1)
		line->pos =	line->pos >= line->lst_input->size ?
			line->lst_input->size - 1 : line->pos;
	refresh_line(line, prompt, 1);
	line->edit_history.index = line->edit_history.len;
	str = convert_to_str(line->lst_input, 0);
	add_history(&(line->edit_history), ft_strdup(str), H_EMPTYL, line->pos);
	free (str);
	str = NULL;
	line->edit = 1;
	while (!g_sigquit && (ret = read(0, line->buff, 1)) >= 0)
	{
		if (g_sigquit)
			break ;
		if (ret > 0)
		{
			lst = ft_lstindex(line->edit_history.cursor_pos,
				line->edit_history.index);
			if (lst)
				lst->content = (void *)((size_t)line->pos);
			if (*line->buff == 27 && handle_escape(line, prompt, 1) == 1)
			{
				sel = 0;
				ft_bzero(line->sel, sizeof(line->sel));
			}
			else if (*line->buff == 'h')
						line->pos -= line->pos > 1 ? 1 : 0;
			else if (*line->buff == 'j' && line->multi)
						line->pos += (line->pos + g_termx < line->lst_input->size) ? g_termx : 0;
			else if (*line->buff == 'k' && line->multi)
						line->pos -= (line->pos - g_termx > 0) ? g_termx : 0;
			else if (*line->buff == 'l')
				line->pos += line->pos < line->lst_input->size ? 1 : 0;
			else if (*line->buff == 'v' && (sel = 1))
				line->sel[0] = line->pos;
			else if (*line->buff == 'p' && !sel)
				past(line, (line->lst_input->size == 1));
			else if (*line->buff == 'P' && !sel)
				past(line, 1);
			else if (*line->buff == 'w')
				go_right(line);
			else if (*line->buff == 'u')
				history_pn(line, HPREV, &(line->edit_history));
			else if (*line->buff == 0x12)
				history_pn(line, HNEXT, &(line->edit_history));
			else if (*line->buff == 'x')
			{
				++line->edit_history.index;
				sel = 0;
				g_yank = get_str_by_pos(line, 1);
				ft_bzero(line->sel, sizeof(line->sel));
				str = convert_to_str(line->lst_input, 0);
				free_history(&line->edit_history, line->edit_history.index);
				add_history(&(line->edit_history), ft_strdup(str),
						H_EMPTYL, line->pos);
				free (str);
				str = NULL;
			}
			else if (*line->buff == 'X')
			{
				++line->edit_history.index;
				sel = 0;
				ft_bzero(line->sel, sizeof(line->sel));
				handle_ctrlu(line);
				str = convert_to_str(line->lst_input, 0);
				free_history(&line->edit_history, line->edit_history.index);
				add_history(&(line->edit_history), ft_strdup(str), H_EMPTYL,
						line->pos);
				free (str);

			}
			else if (*line->buff == 10 && !sel)
				break ;
			else if (*line->buff == 'y' || *line->buff == 10)
			{
				sel = 0;
				g_yank = get_str_by_pos(line, 0);
				ft_bzero(line->sel, sizeof(line->sel));
			}
			if (*line->buff == 'i')
				break ;
			if (*line->buff == 'I' && (line->pos = 1))
				break ;
			if (*line->buff == 'A' && (line->pos = line->lst_input->size))
				break ;
			if (sel)
				line->sel[1] = line->pos < line->sel[0] ? line->pos : line->pos + 1;
			else
				ft_bzero(line->sel, sizeof(line->sel));
			if (line->pos > 1)
				line->pos =	line->pos >= line->lst_input->size ?
					line->lst_input->size - 1 : line->pos;
			if (g_resize && !((g_resize = 0)))
				ft_printf("\r\x1b[0K%s", prompt);
			refresh_line(line, prompt, 1);
			ft_bzero(line->buff, 6);
		}
	}
	line->edit = 0;
	ft_bzero(line->sel, sizeof(line->sel));
	refresh_line(line, prompt, 0);
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
