/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 22:39:41 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/09 15:47:51 by riblanc          ###   ########.fr       */
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
	if (*s1 == *s2 && *s1 == '\0' && *s2 == '\0')
		return (1);
	return (0);
}

int		get_size_current_word(t_data *lst, t_lst_in **tmp, int pos)
{
	int			old_i;
	int			i;

	i = lst->size;
	while (*tmp != NULL && --i > pos)
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

char	*get_current_word(t_data *lst, int pos)
{
	t_lst_in	*tmp;
	char		*str;
	int			i_str;
	int			size;

	tmp = lst->end;
	size = get_size_current_word(lst, &tmp, pos);
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

void	add_str_to_lst(int pos, char *str, char *filename, t_data *lst)
{
	t_lst_in	*tmp;
	int			offset;
	int			size;
	int			i;

	tmp = lst->end;
	size = get_size_current_word(lst, &tmp, pos);
	offset = 0;
	while (str[offset] && offset < size)
		++offset;
	offset -= size == -1;
	while (++offset < ft_strlen(filename))// && tmp && tmp->prev)
	{
		add_after(lst, filename[offset], pos);
	}
}

int		print_highlight(t_shell *sh, t_data *lst, int pos, char *str, int nb_elem, int i)
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
				if (add)
					add_str_to_lst(pos, str, file->d_name, lst);
				else
					size += ft_printf("\e[104m%s\e[0m    ", file->d_name);
			}
			else
				size += ft_printf("%s    ", file->d_name);
			++j;
		}
	}
	if (closedir(rep) == -1)
		return (-1);
	return (size);
}

int		print_match(t_shell *sh, t_data *lst, int pos, int offset)
{
	int				size;
	char			*str;
	int				i;
	int				nb_elem;
	int				ret;

	size = 0;
	str = get_current_word(lst, pos);
	if ((nb_elem = get_nmatch(sh, str)) == -1)
	{
		free(str);
		return (0);
	}
	i = 0;
	while ((ret = read_char()))
	{
		if (ret == 9 || ret == 10)
		{
			ft_printf("\n");
			size = print_highlight(sh, lst, pos, str, nb_elem *
					((ret == 9) ? 1 : -1), i - (ret == 10));
			i += (ret == 9);
			ft_printf("\e[A");
			print_line(lst, pos, offset, &size);
			if (ret == 10)
				break ;
		}
		else
			break ;
	}
	free(str);
	return (size);
}
