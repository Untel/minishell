/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termios.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/22 12:37:47 by riblanc           #+#    #+#             */
/*   Updated: 2020/10/28 07:56:27 by riblanc          ###   ########.fr       */
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
