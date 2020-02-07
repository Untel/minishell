/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termios.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 16:44:22 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/07 19:50:09 by riblanc          ###   ########.fr       */
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

	if (tcsetattr(0, 0, s_termios) == -1)
		return (-1);
    return 0;
}

t_lst	*add_elem(char c, t_lst *next, t_lst *prev)
{
	t_lst *new;

	if (!(new = malloc(sizeof(t_lst))))
		return (NULL);
	new->c = c;
	new->prev = prev;
	new->next = next;
	return (new);
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

char	*read_input(int offset)
{
	int c;
	char test[3];
	t_data *lst;
	int		pos = 1;

	lst = malloc(sizeof(t_data));
	init_lst(lst);
	add_empty(lst, '\0');
	c = 0;
	while (read(0, &c, 3))
	{
		if (c == 127)
		{
			if (lst->size > 1 && pos < lst->size)
				delone(lst, pos + 1);
			write(1, "\e[D", 3);
			write(1, &c, 1);
			write(1, "\e[D", 3);
		}
		else if (c == 4)
		{
			if (lst->size == 1)
			{
				free_all(lst);
				free(lst);
				return ((char *)-1);
			}
			if (pos > 1)
				delone(lst, pos--);
			write(1, &c, 1);
		}
		else if (c > 127)
		{
			test[2] = c >> 16;
			test[1] = (c >> 8) & 0x0000FF;
			test[0] = c & 0x0000FF;
			if (c != 4283163 && c!= 4348699)
			{
				if (c == 4414235)
					if (pos > 1)
						--pos;
				if (c == 4479771)
					if (pos < lst->size)
						++pos;
				write(1, &test[0], 1);
				write(1, &test[1], 1);
				write(1, &test[2], 1);
			}
		}
		else if (c == 10)
		{
			return (convert_to_str(lst));
		}
		else if (c != 9)
			if (lst->size < tgetnum("co") - 1 - offset)
				add_after(lst, c, pos);
		c = 0;
		print_line(lst, pos, offset);
	}
	return (NULL);
}

/*#include <string.h>

int main(int argc, char **argv)
{

	if (init_term(&s_termios, &s_termios_backup) == 0)
	{
		//print_escape_sequence(cl_cap);
			printf("\nreturn: [%s]\n", (str = read_input(ret)));
		}
		free(str);
		system("leaks a.out");
		//printf("return: [%s]\n", read_input(s_termios));
		if (tcsetattr(0, 0, &s_termios_backup) == -1)
			return (-1);
	}
	return ret;
}*/
