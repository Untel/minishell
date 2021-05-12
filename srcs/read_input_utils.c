/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 23:13:15 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 00:43:31 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"
#include "get_next_line.h"
#include <unistd.h>
#include <stdlib.h>

char	*linedit_notty(void)
{
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

int		init_sline(t_line *line)
{
	static char *yank = NULL;

	ft_memset(line, 0, sizeof(*line));
	line->yank = &yank;
	if (line->yank && *line->yank)
		line->copy_buff = *line->yank;
	if (!(line->lst_input = malloc(sizeof(t_data))))
		return (-1);
	init_lst(line->lst_input);
	add_empty(line->lst_input, 0);
	line->pos = 1;
	return (0);
}

char	*free_input(t_line *line, int free_yank)
{
	if (free_yank && line->copy_buff)
	{
		ft_memdel((void **)&(line->copy_buff));
		*line->yank = NULL;
	}
	free_all(line->lst_input);
	ft_memdel((void **)&(line->lst_input));
	free_history(line->input_history, 0);
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
	}
	else if (*s1 && s2)
	{
		tmp = ft_strjoin(*s1, s2);
		free(*s1);
		*s1 = tmp;
	}
}

void	refresh_line(t_line *line, char *prompt, int edit)
{
	if (line->multi)
		refresh_multi_line(line, prompt, edit);
	else
		refresh_single_line(line, edit);
}
