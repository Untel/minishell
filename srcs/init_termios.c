/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termios.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:44:22 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/09 09:29:13 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include "list.h"
#include "minishell.h"

int	init_term(struct termios *s_termios, struct termios *s_termios_backup)
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
