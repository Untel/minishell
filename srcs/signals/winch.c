/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   winch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 10:58:52 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 03:56:36 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include "line_edit.h"
#include <fcntl.h>
#include <sys/ioctl.h>

t_terminfos g_term_infos = {0};

static int		winch(void)
{
	struct winsize ws;

	ioctl(0, TIOCGWINSZ, &ws);
	g_term_infos.height = ws.ws_row;
	g_term_infos.width = ws.ws_col;
	return (0);
}

static void		update_footer(int clear)
{
	char	nb_lines[16];
	char	*buf;

	buf = NULL;
	ft_sprintf(nb_lines, "%d", g_term_infos.height);
	append(&buf, "\x1b[s\x1b[");
	append(&buf, nb_lines);
	append(&buf, ";0H");
	append(&buf, clear ? "\x1b[0K" : "\x1b[1m -- EDIT MODE --\x1b[0m");
	append(&buf, "\x1b[u");
	write(1, buf, strlen(buf));
	free(buf);
}

void			handle_winch(int sig)
{
	int		ret;

	(void)sig;
	g_term_infos.resize = 1;
	signal(SIGWINCH, SIG_IGN);
	if (!MULTI && sig != -1)
		ft_printf("\r\x1b[0Kresizing...\x1b[0K");
	if (g_term_infos.height > 0 && sig != -1 && g_term_infos.is_edit)
		update_footer(1);
	ret = winch();
	if (ret == -1 || !g_term_infos.height || !g_term_infos.width)
		handle_winch(0);
	if (g_term_infos.height > 0 && sig != -1 && g_term_infos.is_edit)
		update_footer(0);
	signal(SIGWINCH, handle_winch);
}
