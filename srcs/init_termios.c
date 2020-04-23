/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termios.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 12:37:47 by riblanc           #+#    #+#             */
/*   Updated: 2020/04/23 15:32:17 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <termios.h>
#include <stdlib.h>
#include "list.h"
#include "libft.h"
#include <unistd.h>
#include <signal.h>
#include "line_edit.h"

int
	init_term(struct termios *s_termios, struct termios *s_termios_backup)
{
	char	*term_type;

	signal(SIGTSTP, SIG_IGN);
	signal(SIGINT, sigquit);
	term_type = "xterm-256color";
	if (term_type == NULL)
		return (-1);
	if (tcgetattr(0, s_termios) == -1)
		return (-1);
	if (tcgetattr(0, s_termios_backup) == -1)
		return (-1);
	s_termios->c_lflag &= ~(ICANON | ECHO);
	s_termios->c_cc[VMIN] = 0;
	s_termios->c_cc[VTIME] = 0;
	if (tcsetattr(0, 0, s_termios) == -1)
		return (-1);
	return (0);
}

int
	snda_toi(char *str)
{
	while (ft_isdigit(*str))
		++str;
	return (ft_atoi(str));
}

int
	get_term_size(char **av, char **env)
{
	int		pid;
	int		p[2];
	char	str[65];
	int		ret;

	pipe(p);
	if ((pid = fork()) == -1)
		return (-1);
	else if (pid == 0)
	{
		dup2(p[1], 1);
		close(p[0]);
		ret = execve("/bin/stty", av, env);
		exit(ret);
	}
	else
	{
		close(p[1]);
		wait(0);
		ret = read(p[0], str, 64);
		str[ret] = 0;
		g_termy = ft_atoi(str);
		g_termx = snda_toi(str);
	}
	return (0);
}

void
	save_cursor_pos(void)
{
	write(1, "\x1b[s", 3);
}
