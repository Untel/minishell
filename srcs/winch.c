/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   winch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/20 10:58:52 by riblanc           #+#    #+#             */
/*   Updated: 2020/05/12 23:24:48 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include "line_edit.h"
#include <fcntl.h>

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
