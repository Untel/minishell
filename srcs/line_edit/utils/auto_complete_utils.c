/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_complete_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/13 01:29:11 by riblanc           #+#    #+#             */
/*   Updated: 2020/10/13 01:30:56 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"

int		get_size_cword(t_lst_in *tmp)
{
	int		i;

	i = 0;
	while (tmp && !ft_isspace(tmp->c))
	{
		++i;
		tmp = tmp->next;
	}
	return (i);
}

char	*get_current_word(t_line *line)
{
	t_lst_in	*tmp;
	char		*str;
	int			size;
	int			i;

	tmp = get_elem_by_pos(line->lst_input, line->pos - 1);
	if (ft_isspace(tmp->c) || !tmp->c)
		return (NULL);
	while (line->pos > 0 && tmp && tmp->c && !ft_isspace(tmp->c))
	{
		--line->pos;
		tmp = tmp->prev;
	}
	tmp = tmp->next;
	size = get_size_cword(tmp);
	if (!size || !(str = malloc(sizeof(char) * (size + 1))))
		return (NULL);
	str[size] = 0;
	i = 0;
	while (--size >= 0)
	{
		str[i++] = tmp->c;
		tmp = tmp->next;
	}
	return (str);
}

char	**split_path(char *str)
{
	char	**strs;
	int		last_slash;
	int		i;

	last_slash = -1;
	i = -1;
	while (str && *(str + ++i))
		if (*(str + i) == '/')
			last_slash = i;
	if (!(strs = malloc(sizeof(char *) * 2)))
		return (NULL);
	if (last_slash < 0 || !str)
		strs[0] = ft_strdup("./");
	else
		strs[0] = ft_substr(str, 0, last_slash + 1);
	if (str)
		strs[1] = ft_strdup(str + last_slash + 1);
	else
		strs[1] = ft_strdup("");
	return (strs);
}

int		ft_strslen(char **strs)
{
	int		i;

	i = 0;
	if (strs)
		while (strs[i])
			++i;
	return (i);
}

void	get_rest_word(t_line *line, char *match, char *elem)
{
	int		s_match;
	int		s_elem;

	(void)line;
	s_match = ft_strlen(match) - 1;
	s_elem = ft_strlen(elem);
	if (line->complete.str)
		free(line->complete.str);
	line->complete.str = ft_substr(elem, s_match, s_elem - s_match);
	line->complete.pos = line->pos;
	refresh_line(line, line->prompt, 0);
}
