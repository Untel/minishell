/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/08 22:39:41 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/09 08:24:50 by riblanc          ###   ########.fr       */
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

int		print_match(t_shell *sh, t_data *lst, int pos)
{
	struct dirent	*file;
	DIR				*rep;
	int				size;
	char			*str;

	file = NULL;
	rep = NULL;
	size = 0;
	str = get_current_word(lst, pos);
	ft_printf("\n");
	if ((rep = opendir(sh->dir)) == NULL)
	{
		free(str);
		return (-1);
	}
	while ((file = readdir(rep)) != NULL)
		if (match(file->d_name, str))
			size += ft_printf("%s    ", file->d_name);
	free(str);
	if (closedir(rep) == -1)
		return (-1);
	ft_printf("\e[A");
	return (size);
}
