/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   winch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 10:58:52 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/11 20:02:14 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include "line_edit.h"
#include <fcntl.h>
#include <sys/ioctl.h>

static int		winch(void)
{
	struct winsize ws;
	ioctl(0, TIOCGWINSZ, &ws);
	g_termy = ws.ws_row;
	g_termx = ws.ws_col;
	return (0);
}

void			handle_winch(int sig)
{
	int		ret;

	(void)sig;
	g_resize = 1;
	signal(SIGWINCH, SIG_IGN);
	if (!MULTI && sig != -1)
		ft_printf("\r\x1b[0Kresizing...\x1b[0K");
	ret = winch();
	if (ret == -1 || !g_termx || !g_termy)
		handle_winch(0);
	signal(SIGWINCH, handle_winch);
}
