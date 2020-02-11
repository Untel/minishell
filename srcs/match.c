/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 22:39:41 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/11 04:59:09 by riblanc          ###   ########.fr       */
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

int		get_size_current_word(t_shell *sh, t_lst_in **tmp)
{
	int			old_i;
	int			i;

	i = sh->term.input->size;
	while (*tmp != NULL && --i > sh->term.pos_str)
		*tmp = (*tmp)->prev;
	old_i = i;
	while (*tmp && (*tmp)->next && (*tmp)->c != ' ')
	{
		*tmp = (*tmp)->next;
		++i;
	}
	if ((*tmp)->c == ' ' && --i)
		*tmp = (*tmp)->prev;
	return (i - old_i);
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

int		read_char(void)
{
	char	c;

	c = 0;
	read(0, &c, 1);
	return (c);
}

int		get_nmatch(t_shell *sh, char *str)
{
	DIR				*rep;
	struct dirent	*file;
	int				nb_elem;

	file = NULL;
	rep = NULL;
	nb_elem = 0;
	if ((rep = opendir(sh->dir)) == NULL)
		return (-1);
	while ((file = readdir(rep)) != NULL)
		if (match(file->d_name, str))
			++nb_elem;
	if (closedir(rep) == -1)
		return (-1);
	return (nb_elem);
}

void	add_str_to_lst(t_shell *sh, char *str, char *filename)
{
	t_lst_in	*tmp;
	int			offset;
	int			size;
	int			i;

	tmp = sh->term.input->end;
	i = -1;
	while (++i < sh->term.old_s_in && sh->term.input->size > 2)
		delone(sh->term.input, sh->term.pos_str + 1);
	size = get_size_current_word(sh, &tmp);
	offset = 0;
	while (str[offset] && offset < size)
		++offset;
	sh->term.old_s_in = ft_strlen(filename) - offset;
	offset -= (size < 0 || (size == 0 && (tmp->c == 32 || tmp->c == 0)));
	while (++offset < ft_strlen(filename))
		add_after(sh->term.input, filename[offset], sh->term.pos_str);
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
	j = 0;
	size = 0;
	add = nb_elem < 0 ? 1 : 0;
	nb_elem *= nb_elem < 0 ? -1 : 1;
	if ((rep = opendir(sh->dir)) == NULL)
		return (-1);
	while ((file = readdir(rep)) != NULL)
	{
		if (match(file->d_name, str))
		{
			if (j % nb_elem == i % nb_elem)
			{
				add_str_to_lst(sh, str, file->d_name);
				size += ft_printf("\e[104m%s\e[0m    ", file->d_name);
			}
			else
				size += ft_printf("%s    ", file->d_name);
			++j;
		}
	}
	if (closedir(rep) == -1)
		return (-1);
	return (size + 10);
}

int		print_match(t_shell *sh)
{
	int				size;
	char			*str;
	int				i;
	int				j;
	int				nb_elem;
	int				ret;

	size = 0;
	str = get_current_word(sh);
	if ((nb_elem = get_nmatch(sh, str)) == -1)
	{
		free(str);
		return (0);
	}
	i = 0;
	sh->term.old_s_in = 0;
	while ((ret = read_char()))
	{
		if (ret == 9 || ret == 10)
		{
			ft_printf("\n");
			size = print_highlight(sh, str, nb_elem * ((ret == 9) ? 1 : -1),
					i - (ret == 10));
			i += (ret == 9);
			j = -1;
			while (++j <= (size / g_termx) - (size > g_termx && (size /g_termx) == 1))
				ft_printf("\e[A");
			print_line(sh, &size);
			if (ret == 10)
				break ;
		}
		else
			break ;
	}
	sh->term.pos_str *= -1;
	print_line(sh, &size);
	free(str);
	return (size);
}
