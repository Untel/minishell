/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   winch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 10:58:52 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/29 18:47:20 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include "line_edit.h"
#include "ft_printf.h"
#include <fcntl.h>

int			g_termx = 0;
int			g_termy = 0;
int			g_resize = 0;

void	handle_winch(int sig)
{
	char	*av[3];
	int		dev_null;
	int		stderr_save;

	dev_null = open("/dev/null", O_WRONLY);
	stderr_save = dup(2);
	dup2(dev_null, 2);
	g_resize = 1;
	signal(SIGWINCH, SIG_IGN);
	av[0] = "stty";
	av[1] = "size";
	av[2] = 0;
	get_term_size(av, NULL);
	(void)sig;
	signal(SIGWINCH, handle_winch);
	dup2(stderr_save, 2);
	close(dev_null);
}
