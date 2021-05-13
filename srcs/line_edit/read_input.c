/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 00:22:31 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 01:29:20 by riblanc          ###   ########.fr       */
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

char
	*init_read(t_line *line, char *prompt, int multi, int size_prompt)
{
	init_sline(line);
	line->multi = multi;
	line->prompt = prompt;
	line->size_prompt = size_prompt;
	init_term(&(line->s_term), &(line->s_term_backup));
	if (!g_term_size.width)
	{
		handle_winch(-1);
		if (!g_term_size.width)
			return (linedit_notty());
	}
	line->ret = 0;
	line->nb_res = 0;
	write(1, line->prompt, ft_strlen(line->prompt));
	line->complete.pos = -1;
	line->complete.str = 0;
	refresh_line(line, line->prompt, 0);
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
				add_history(line->input_history, ft_strdup(line->str), H_SAVE,
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
	if (g_term_size.resize && !(g_term_size.resize = 0))
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

	if ((line.str = init_read(&line, prompt, multi, size_prompt)) != (char *)-1)
		return (line.str);
	line.input_history = (t_history *)&sh->cmd_history;
	line.input_history->index = line.input_history->len
		? line.input_history->len : 1;
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
