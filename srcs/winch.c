/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   winch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 10:58:52 by riblanc           #+#    #+#             */
/*   Updated: 2020/10/28 15:42:38 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include "line_edit.h"
#include <fcntl.h>

static int		ft_limited_atoi(char begin, char *str, char end)
{
	int		res;
	int		i;

	i = 0;
	res = 0;
	while (*(str + i) && *(str + i++) != begin)
		;
	while (*(str + i) && *(str + i) != end)
		res = (res * 10) + (*(str + i++) - 48);
	return (res);
}

static int		get_cursor_pos(int pos[2])
{
	char	ch;
	char	buf[31];
	int		ret;
	int		i;

	i = -1;
	ch = 0;
	write(1, "\x1b[6n", 4);
	while (ch != 'R' && ++i < 30)
	{
		ret = read(0, &ch, 1);
		if (!ret)
			return (-1);
		buf[i] = ch;
	}
	buf[i] = 0;
	pos[0] = ft_limited_atoi('[', buf, ';');
	pos[1] = ft_limited_atoi(';', buf, 'R');
	return ((!pos[0] || !pos[1]) ? -1 : 0);
}

static int		winch(void)
{
	int pos[2];
	int i;

	i = -1;
	write(1, "\x1b""7", 2);
	while (++i < 9999)
		write(1, "\x1b[C\x1b[B", 6);
	if (get_cursor_pos(pos))
		return (-1);
	write(1, "\x1b""8", 2);
	g_termy = pos[0];
	g_termx = pos[1];
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
