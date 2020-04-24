/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 00:22:31 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/24 18:09:02 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"
#include "list.h"
#include "libft.h"
#include "ft_printf.h"
#include "get_next_line.h"
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

extern int	g_termx;
extern int	g_termy;
extern int	g_resize;
extern char	*g_yank;

static char *linedit_notty(void) {
	char	*line;
	int		ret;

	ret = get_next_line(0, &line);
	if (ret > 0)
		return (line);
	else if (ret == 0)
	{
		free(line);
		return (NULL);
	}
	else
		return (NULL);
}

static int init_sline(t_line *line)
{
	ft_memset(line, 0, sizeof(*line));
	if (g_yank)
		line->copy_buff = g_yank;
	if (!(line->lst_input = malloc(sizeof(t_data))))
		return (-1);
	init_lst(line->lst_input);
	add_empty(line->lst_input, 0);
	line->pos = 1;
	return (0);
}

void	nprint(char *str, int size)
{
	while (--size >= 0)
		write(1, str, ft_strlen(str));
}

static char	*free_input(t_line *line, int free_yank)
{
	if (free_yank && line->copy_buff)
	{
		ft_memdel((void **)&(line->copy_buff));
		g_yank = NULL;
	}
	free_all(line->lst_input);
	ft_memdel((void **)&(line->lst_input));
	free_history(&g_history, 0);
	free_history(&(line->edit_history), 0);
	write(1, "\n", 1);
	return ((char *)-1);
}

void	append(char **s1, char *s2)
{
	char	*tmp;

	if (!(*s1) && !s2)
		return ;
	else if (!(*s1) && s2)
	{
		*s1 = ft_strdup(s2);
		ft_memdel((void **)&s2);
	}
	else if (*s1 && s2)
	{
		tmp = ft_strjoin(*s1, s2);
		ft_memdel((void **)&s2);
		free(*s1);
		*s1 = tmp;
	}
}

void	refresh_single_line(t_line *line, int edit)
{
	int		pos = line->pos;
	int		len = line->lst_input->size;
	int		buf = 0;
	int		offset = line->size_prompt % g_termx;

	line->buf = NULL;
	append(&line->buf, ft_strdup("\n\x1b[s\x1b[0m\x1b[999B\r\x1b[0K"));
	if (edit)
		append(&line->buf, ft_strdup("\x1b[1m -- EDIT MODE --"));
	append(&line->buf, ft_strdup("\x1b[u\x1b[A"));
	while ((offset + pos) >= g_termx)
	{
		++buf;
		--pos;
		--len;
	}
	while ((offset + len) > g_termx)
		--len;
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

void	refresh_multi_line(t_line *line, char *prompt, int edit)
{
	int		offset = line->size_prompt;
	int		len = line->lst_input->size - 1;
	int		rows = (offset + len + g_termx - 1) / g_termx;
	int		pos =  line->pos - 1;
	int		rpos = (offset + line->old_pos + g_termx) / g_termx;
	int		rpos2;
	int		col;
	int		old_rows = line->maxrows;
	int		i;

	if (rows > line->maxrows)
		line->maxrows = rows;
	line->buf = NULL;
	i = rpos;
	while (++i <= rows)
		append(&line->buf, ft_strdup("\x1b[B"));
	append(&line->buf, ft_strdup("\n\x1b[s\x1b[0m\x1b[999B\r\x1b[0K"));
	if (edit)
		append(&line->buf, ft_strdup("\x1b[1m -- EDIT MODE --"));
	append(&line->buf, ft_strdup("\x1b[u\x1b[A"));
	i = rpos;
	while (++i <= rows)
		append(&line->buf, ft_strdup("\x1b[A"));
	if(old_rows - rpos > 0)
	{
		ft_sprintf(line->seq, "\x1b[%dB", old_rows - rpos);
		append(&line->buf, ft_strdup(line->seq));
	}
	i = -1;
	while (++i < old_rows - 1)
		append(&line->buf, ft_strdup("\r\x1b[0K\x1b[1A"));
	append(&line->buf, ft_strdup("\r\x1b[0K"));
	append(&line->buf, ft_strdup(prompt));
	append_multi_cmd(line, len);
	append(&line->buf, ft_strdup("\x1b[0m"));
	if (pos && pos == len && (pos + offset) % g_termx == 0)
	{
		append(&line->buf, ft_strdup("\n\r"));
		++rows;
		if (rows > line->maxrows)
			line->maxrows = rows;
	}
	rpos2 = (offset + pos + g_termx) / g_termx;
	if (rows - rpos2 > 0)
	{
		ft_sprintf(line->seq, "\x1b[%dA", rows - rpos2);
		append(&line->buf, ft_strdup(line->seq));
	}
	col = (offset + pos) % g_termx;
	if (col)
		ft_sprintf(line->seq, "\r\x1b[%dC", col);
	else
		ft_sprintf(line->seq, "\r");
	append(&line->buf, ft_strdup(line->seq));
	line->old_pos = pos;
	write(1, line->buf, ft_strlen(line->buf));
	ft_memdel((void **)&line->buf);
}

void	refresh_line(t_line *line, char *prompt, int edit)
{
	if (line->multi)
		refresh_multi_line(line, prompt, edit);
	else
		refresh_single_line(line, edit);
}

char		*read_input(char *prompt, int multi, int size_prompt)
{
	t_line	line;
	int		ret;
	char	*str;

	handle_winch(-1);
	if (!g_termx)
		return linedit_notty();
	init_sline(&line);
	init_term(&(line.s_term), &(line.s_term_backup));
	line.multi = multi;
	line.size_prompt = size_prompt;
	ft_printf("\x1b[0m\x1b[7m%%\x1b[27m%*s\r", g_termx - 1 , "");
	ft_printf(prompt);
	refresh_line(&line, prompt, 0);
	g_history.index = g_history.len ? g_history.len : 1;
	while ((ret = read(0, line.buff, 1)) >= 0)
	{
		if (ret > 0)
		{
			if (g_resize && !((g_resize = 0)))
				ft_printf("\r\x1b[0K%s", prompt);
			line.old_size = line.lst_input->size;
			if ((ret = handle_input(&line, prompt)))
			{
				tcsetattr(0, 0, &(line.s_term_backup));
				if (ret == -1)
					return (free_input(&line, 1));
				else
				{
					str = convert_to_str(line.lst_input, 1);
					add_history(&g_history, ft_strdup(str), H_SAVE,
							ft_strlen(str) + 1);
					write(1, "\n", 1);
					free_history(&(line.edit_history), 0);
					return (str);
				}
			}
			refresh_line(&line, prompt, 0);
			ft_bzero(line.buff, 6);
		}
	}
	tcsetattr(0, 0, &(line.s_term_backup));
	return (NULL);
}
