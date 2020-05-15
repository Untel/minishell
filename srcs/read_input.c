/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 00:22:31 by riblanc           #+#    #+#             */
/*   Updated: 2020/05/15 22:47:25 by riblanc          ###   ########.fr       */
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

int			g_termx = 0;
int			g_termy = 0;
int			g_resize = 0;

char	*init_read(t_line *line, int multi, char *prompt, int size_prompt)
{
	handle_winch(-1);
	if (!g_termx)
		return (linedit_notty());
	init_sline(line);
	init_term(&(line->s_term), &(line->s_term_backup));
	line->multi = multi;
	line->prompt = prompt;
	line->size_prompt = size_prompt;
	ft_printf(prompt);
	line->complete.pos = -1;
	line->complete.str = 0;
	refresh_line(line, prompt, 0);
	g_history.index = g_history.len ? g_history.len : 1;
	return ((char *)-1);
}

#include "minishell.h"

char	*check_handle(t_shell *sh, t_line *line, char *prompt, char *str, int ret)
{
	if ((ret = handle_input(sh, line, prompt)))
	{
		tcsetattr(0, 0, &(line->s_term_backup));
		if (ret == -1)
			return (free_input(line, 1));
		else
		{
			if (ret != 2)
			{
				str = convert_to_str(line->lst_input, 1);
				add_history(&g_history, ft_strdup(str), H_SAVE,
					ft_strlen(str) + 1);
			}
			else if (ret == 2)
			{
				free_all(line->lst_input);
				ft_memdel((void **)&(line->lst_input));
			}
			write(1, "\n", 1);
			free_history(&(line->edit_history), 0);
			return (ret != 2 ? str : (char *)-3);
		}
	}
	return ((char *)-2);
}

char	*read_input(t_shell *sh, char *prompt, int multi, int size_prompt)
{
	t_line	line;
	int		ret;
	char	*str;

	ret = 0;
	if ((str = init_read(&line, multi, prompt, size_prompt)) != (char *)-1)
		return (str);
	str = 0;
	while (g_resize || (ret = read(0, line.buff, 1)) >= 0)
	{
		if (g_resize)
			g_resize = 0;
		if (ret > 0)
		{
			line.old_size = line.lst_input->size;
			if ((str = check_handle(sh, &line, prompt, str, ret)) != (char *)-2)
				return (str);
		}
		refresh_line(&line, prompt, 0);
		ft_bzero(line.buff, 6);
	}
	tcsetattr(0, 0, &(line.s_term_backup));
	return (NULL);
}
