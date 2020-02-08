/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termios.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:44:22 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/08 18:40:26 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <term.h>
#include <unistd.h>
#include "list.h"

typedef	struct	s_lst
{
	char	c;
	void	*prev;
	void	*next;
}				t_lst;

int init_term(struct termios *s_termios, struct termios *s_termios_backup)
{
    int ret;
    char *term_type = getenv("TERM");

    if (term_type == NULL)
        return -1;
    ret = tgetent(NULL, term_type);
    if (ret == -1)
        return -1;
    else if (ret == 0)
        return -1;
	if (tcgetattr(0, s_termios) == -1)
		return (-1);

	if (tcgetattr(0, s_termios_backup) == -1)
		return (-1);

	s_termios->c_lflag &= ~(ICANON);
	s_termios->c_lflag &= ~(ECHO);
	s_termios->c_cc[VMIN]     = 1;

	if (tcsetattr(0, 0, s_termios) == -1)
		return (-1);
    return 0;
}

void	print_line(t_data *lst, int pos, int offset)
{
	int			i;
	int			del;
	static int	old_size = 0;

	del = 127;
	i = -1;
	while (++i < 2 + old_size - lst->size)
		write(1, &del, 1);
	write(1, "\r", 1);
	while (--offset >= 0)
		write(1, "\e[C", 3);
	affiche_inv(lst);
	write(1, &del , 1);
	i = -1;
	while (++i < pos)
		write(1, "\e[D", 3);
	old_size = lst->size;
}

char	*convert_to_str(t_data *lst)
{
	t_lst_in	*tmp;
	char		*new;
	int			i;

	tmp = lst->end;
	if (!(new = malloc(sizeof(char) * (lst->size + 1))))
		return (NULL);
	i = 0;
	while (tmp != NULL)
	{
		new[i++] = tmp->c;
		tmp = tmp->prev;
	}
	free_all(lst);
	free(lst);
	return (new);
}

void	handle_arrows(char buff[3], int *pos, t_data *lst)
{
	read(0, buff + 1, 1);
	read(0, buff + 2, 1);
	if (!(buff[1] == '[' && buff[2] == 'A') &&
			!(buff[1] == '[' && buff[2] == 'B'))
	{
		if (buff[1] == '[' && buff[2] == 'C')
		{
			write(1, buff, 3 * (*pos > 1));
			*pos = (*pos > 1) ? *pos - 1 : *pos;
		}
		else if (buff[1] == '[' && buff[2] == 'D')
		{
			write(1, buff, 3 * (*pos < lst->size));
			*pos = (*pos < lst->size) ? *pos + 1 : *pos;
		}
	}
}

void	handle_backspace(char buff[3], int *pos, t_data *lst)
{
	if (lst->size > 1 && *pos < lst->size)
				delone(lst, *pos + 1);
	write(1, "\e[D", 3);
	write(1, buff, 1);
	write(1, "\e[D", 3);
}

int		handle_ctrl_d(char buff[3], int *pos, t_data *lst)
{
	if (lst->size == 1)
	{
		free_all(lst);
		free(lst);
		return (-1);
	}
	if (*pos > 1)
	{
		delone(lst, *pos);
		*pos -= 1;
	}
	write(1, buff, 1);
	return (1);
}

char	*read_input(int offset)
{
	char	buff[3];
	int		ret;
	t_data *lst;
	int		pos;

	lst = malloc(sizeof(t_data));
	init_lst(lst);
	add_empty(lst, '\0');
	pos = 1;
	while ((ret = read(0, buff, 1)))
	{
		if (buff[0] == 27)
			handle_arrows(buff, &pos, lst);
		else if (buff[0] == 127)
			handle_backspace(buff, &pos, lst);
		else if (buff[0] == 4)
		{
			if (handle_ctrl_d(buff, &pos, lst) == -1)
				return ((char *)-1);
		}
		else if (buff[0] == 10)
			return (convert_to_str(lst));
		else if (buff[0] != 9)
			add_after(lst, buff[0], pos);
		print_line(lst, pos, offset);
	}
}
/*
#include <string.h>

int main(int argc, char **argv)
{
	struct termios	s_termios;
	struct termios	s_termios_backup;
	char			*ret;

	if (init_term(&s_termios, &s_termios_backup) == 0)
	{
		//print_escape_sequence(cl_cap);
		while (1)
		{
			ret = _read();
			if (ret == (char *)-1)
				break ;
			printf("\n%s\n", ret);
		}
		//printf("return: [%s]\n", read_input(s_termios));
		if (tcsetattr(0, 0, &s_termios_backup) == -1)
			return (-1);
	}
	return (0);
}*/
