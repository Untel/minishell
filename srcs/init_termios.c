/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termios.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 12:37:47 by riblanc           #+#    #+#             */
/*   Updated: 2020/09/29 16:39:20 by riblanc          ###   ########.fr       */
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
	term_type = "xterm-256color";
	if (term_type == NULL)
		return (-1);
	if (tcgetattr(0, s_termios) == -1)
		return (-1);
	if (tcgetattr(0, s_termios_backup) == -1)
		return (-1);
	s_termios->c_lflag &= ~(ICANON | ECHO);
	s_termios->c_cc[VMIN] = 0;
	s_termios->c_cc[VTIME] = 1;
	s_termios->c_cc[VINTR] = 0;
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

void
	get_size_child(int p[2], int *ret, char **av, char **env)
{
	dup2(p[1], 1);
	close(p[0]);
	*ret = execve("/bin/stty", av, env);
	close(p[1]);
	exit(*ret);
}

int
	get_term_size(char **av, char **env)
{
	int		pid;
	int		p[2];
	char	str[65];
	int		ret;

	ret = pipe(p);
	if (ret == -1 || (pid = fork()) == -1)
		return (-1);
	else if (pid == 0)
		get_size_child(p, &ret, av, env);
	else
	{
		close(p[1]);
		wait(0);
		ret = read(p[0], str, 64);
		str[ret] = 0;
		g_termy = ft_atoi(str);
		g_termx = snda_toi(str);
		close(p[0]);
	}
	return (ret == 0 ? -1 : ret);
}
