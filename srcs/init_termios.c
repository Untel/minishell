/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termios.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:44:22 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/27 17:51:12 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include "list.h"
#include "minishell.h"

int
	init_term(struct termios *s_termios, struct termios *s_termios_backup)
{
	int		ret;
	char	*term_type;

	term_type = "xterm-256color";
	if (term_type == NULL || (ret = tgetent(NULL, term_type)) == -1)
		return (-1);
	if (ret == 0)
		return (-1);
	if (tcgetattr(0, s_termios) == -1)
		return (-1);
	if (tcgetattr(0, s_termios_backup) == -1)
		return (-1);
	s_termios->c_lflag &= ~(ICANON | ECHO);
	s_termios->c_cc[VMIN] = 1;
	if (tcsetattr(0, 0, s_termios) == -1)
		return (-1);
	return (0);
}

int
	get_termx(t_shell *sh, char **av, char **env)
{
	int		pid;
	int		p[2];
	char	str[11];
	int		ret;

	(void)sh;
	pipe(p);
	if ((pid = fork()) == -1)
		return (-1);
	else if (pid == 0)
	{
		dup2(p[1], 1);
		close(p[0]);
		ret = execve("/usr/bin/tput", av, env);
		exit(ret);
	}
	else
	{
		close(p[1]);
		wait(0);
		ret = read(p[0], str, 10);
		str[ret] = 0;
		return (ft_atoi(str));
	}
	return (0);
}
