/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 22:39:41 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/13 07:15:36 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <ncurses.h>
#include <termios.h>

extern int	g_termx;

int		match(char *s1, char *s2)
{
	if (*s1 != '\0' && *s2 == '*')
		return (match(s1 + 1, s2) || match(s1, s2 + 1));
	if (*s1 == '\0' && *s2 == '*')
		return (match(s1, s2 + 1));
	if (*s1 == *s2 && *s1 != '\0' && *s2 != '\0')
		return (match(s1 + 1, s2 + 1));
	if (*s1 == *s2 && *s1 == '\0')
		return (1);
	return (0);
}

int		print_highlight(t_shell *sh, char *str, int nb_elem, int i)
{
	DIR				*rep;
	struct dirent	*file;
	int				j;
	int				size;
	int				add;

	file = NULL;
	rep = NULL;
	j = -1;
	size = 0;
	if ((rep = opendir(sh->dir)) == NULL)
		return (-1);
	while ((file = readdir(rep)) != NULL)
		if (match(file->d_name, str))
		{
			if (++j % nb_elem == i % nb_elem)
				add_str_to_lst(sh, str, file->d_name);
			size += ft_strlen(file->d_name);
		}
	if (closedir(rep) == -1)
		return (-1);
	return (size);
}

int		handle_match(t_shell *sh, char buff[3], int nb_elem, char *str)
{
	int		size;
	int		i;
	char	c;

	size = 0;
	i = 0;
	while ((buff[0] == 9 || buff[0] == 10) && !sh->ctrl_c)
	{
		c = get_elem_by_pos(sh->term.input, sh->term.pos_str - 1)->c;
		if ((c != 0 && c != 32) || sh->term.pos_str == sh->term.input->size)
			return (0);
		size = print_highlight(sh, str, nb_elem * ((buff[0] == 9) ? 1 : -1),
				i - (buff[0] == 10));
		i += (buff[0] == 9);
		if (!size)
			print_list(sh);
		print_line(sh);
		if (buff[0] == 10)
			break ;
		read(0, buff, 1);
	}
	sh->ctrl_c = 0;
	print_line(sh);
	free(str);
	return (size);
}

char	*get_current_word(t_shell *sh)
{
	t_lst_in	*tmp;
	char		*str;
	int			i_str;
	int			size;

	tmp = sh->term.input->end;
	size = get_size_current_word(sh, &tmp);
	if (!(str = malloc(sizeof(char) * (size + 3))))
		return (NULL);
	i_str = -1;
	while (++i_str <= size && tmp->prev)
	{
		str[i_str] = tmp->c;
		tmp = tmp->prev;
	}
	str[i_str] = '*';
	str[i_str + 1] = 0;
	return (str);
}

int		print_match(t_shell *sh, char buff[3])
{
	char			*str;
	int				nb_elem;

	str = get_current_word(sh);
	if ((nb_elem = get_nmatch(sh, str)) == -1)
	{
		free(str);
		return (0);
	}
	sh->term.old_s_in = 0;
	return (handle_match(sh, buff, nb_elem, str));
}
