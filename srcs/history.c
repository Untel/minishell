/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/21 23:15:52 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/28 00:11:45 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "ft_printf.h"
#include "libft.h"
#include "line_edit.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

t_history	g_history = {.filename = ".history"};

void	add_history(t_history *history, char *cmd, int arg, size_t pos)
{
	if ((strcmp(cmd, "") || (arg & H_EMPTYL) == H_EMPTYL)
				&& !ft_lstlast(history->lst))
	{
		ft_lstadd_back(&(history->lst), ft_lstnew(cmd, sizeof(char*)));
		ft_lstadd_back(&(history->cursor_pos),
				ft_lstnew((void*)pos, sizeof(int)));
		++history->len;
		if ((arg & H_SAVE) == H_SAVE)
			history_save(history->filename, cmd);
	}
	else if ((strcmp(cmd, "") || (arg & H_EMPTYL) == H_EMPTYL) &&
			strcmp((char *)(ft_lstlast(history->lst)->content), cmd))
	{
		ft_lstadd_back(&(history->lst), ft_lstnew(cmd, sizeof(char*)));
		ft_lstadd_back(&(history->cursor_pos),
				ft_lstnew((void *)pos, sizeof(int)));
		++history->len;
		if ((arg & H_SAVE) == H_SAVE)
			history_save(history->filename, cmd);
	}
	else
		ft_memdel((void **)&cmd);
}

void	load_history(const char *filename, t_history *history)
{
	int		fd;
	int		ret;
	char	*cmd;

	if ((fd = open(filename, O_RDWR | O_CREAT, 0664)) == -1)
		return ;
	add_history(history, ft_strdup(""), H_NONE, 1);
	while ((ret = get_next_line(fd, &cmd)) > 0)
	{
		add_history(history, ft_strdup(cmd), H_NONE, ft_strlen(cmd) + 1);
		ft_memdel((void **)&cmd);
	}
	if (ret == 0)
		ft_memdel((void **)&cmd);
	close(fd);
}

void	actualize_cmd(t_line *line, char *str)
{
	while (line->lst_input->size > 0)
	{
		delone(line->lst_input, 1);
		--line->pos;
	}
	add_empty(line->lst_input, 0);
	--str;
	while (*(++str))
	{
		add_end(line->lst_input, *str);
		++line->pos;
	}
	++line->pos;
}

int		history_save(const char *filename, char *cmd)
{
	int		fd;
	char	buf[4096];

	if ((fd = open(filename, O_RDWR)) == -1)
		return (-1);
	while (read(fd, buf, 4096) > 0)
		;
	write(fd, cmd, ft_strlen(cmd));
	write(fd, "\n", 1);
	close(fd);
	return (0);
}

void	history_pn(t_line *line, int sens, t_history *history)
{
	char	*str;
	t_list	*tmp;
	t_list	*pos;

	if (history->len > 0)
	{
		if (sens == 0)
			history->index -= history->index > 0 ? 1 : 0;
		else if (sens == 1)
			history->index += history->index < history->len - 1 ? 1 : 0;
		tmp = ft_lstindex(history->lst, history->index);
		pos = ft_lstindex(history->cursor_pos, history->index);
		if (tmp)
		{
			str = (char*)(tmp->content);
			actualize_cmd(line, str);
		}
		if (pos)
			line->pos = (size_t)(pos->content);
	}
}
