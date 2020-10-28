/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 00:22:31 by riblanc           #+#    #+#             */
/*   Updated: 2020/10/28 07:49:31 by riblanc          ###   ########.fr       */
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
#include "minishell.h"

int			g_termx = 0;
int			g_termy = 0;
int			g_resize = 0;

char
	*init_read(t_line *line, int multi, char *prompt, int size_prompt)
{
	init_sline(line);
	init_term(&(line->s_term), &(line->s_term_backup));
	if (!g_termx)
	{
		handle_winch(-1);
		if (!g_termx)
			return (linedit_notty());
	}
	line->ret = 0;
	line->nb_res = 0;
	line->multi = multi;
	line->prompt = prompt;
	line->size_prompt = size_prompt;
	write(1, prompt, ft_strlen(prompt));
	line->complete.pos = -1;
	line->complete.str = 0;
	refresh_line(line, prompt, 0);
	g_history.index = g_history.len ? g_history.len : 1;
	return ((char *)-1);
}

char
	*check_handle(t_shell *sh, t_line *line, char *prompt)
{
	int		ret;

	if ((ret = handle_input(sh, line, prompt)))
	{
		if ((tcsetattr(0, 0, &(line->s_term_backup)) || 1) && ret == -1)
			return (free_input(line, 1));
		else
		{
			if (ret != 2)
			{
				line->str = convert_to_str(line->lst_input, 1);
				add_history(&g_history, ft_strdup(line->str), H_SAVE,
					ft_strlen(line->str) + 1);
			}
			else if (ret == 2)
			{
				free_all(line->lst_input);
				ft_memdel((void **)&(line->lst_input));
			}
			write(1, "\n", 1);
			free_history(&(line->edit_history), 0);
			return (ret != 2 ? line->str : (char *)-3);
		}
	}
	return ((char *)-2);
}

static int
	manage_resize(int nb_res, t_line *line, char *prompt)
{
	if (g_resize && !(g_resize = 0))
	{
		if (!MULTI)
		{
			ft_printf("\r\x1b[0K");
			write(1, prompt, ft_strlen(prompt));
			if (nb_res != 0)
			{
				refresh_line(line, prompt, 0);
				ft_bzero(line->buff, 6);
			}
		}
		return (1);
	}
	return (0);
}

char
	*read_input(t_shell *sh, char *prompt, int multi, int size_prompt)
{
	t_line	line;

	if ((line.str = init_read(&line, multi, prompt, size_prompt)) != (char *)-1)
		return (line.str);
	while ((line.ret = read(0, line.buff, 1)) >= 0)
	{
		if (line.ret > 0)
		{
			if (manage_resize(line.nb_res, &line, prompt))
				++line.nb_res;
			line.old_size = line.lst_input->size;
			if ((line.str = check_handle(sh, &line, prompt)) != (char *)-2)
			{
				tcsetattr(0, 0, &(line.s_term_backup));
				return (line.str);
			}
			refresh_line(&line, prompt, 0);
			ft_bzero(line.buff, 6);
		}
	}
	tcsetattr(0, 0, &(line.s_term_backup));
	return (NULL);
}
