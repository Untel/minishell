/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   winch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 10:58:52 by riblanc           #+#    #+#             */
/*   Updated: 2020/05/28 17:37:52 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include "line_edit.h"
#include <fcntl.h>

void	handle_winch(int sig)
{
	char	*av[3];
	int		ret;

	(void)sig;
	g_resize = 1;
	signal(SIGWINCH, SIG_IGN);
	av[0] = "stty";
	av[1] = "size";
	av[2] = 0;
	if (!MULTI && sig != -1)
		ft_printf("\r\x1b[0Kresizing...\x1b[0K");
	ret = get_term_size(av, NULL);
	if (ret == -1 || !g_termx || !g_termy)
		handle_winch(0);
	signal(SIGWINCH, handle_winch);
}
