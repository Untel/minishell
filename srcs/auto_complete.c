/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_complete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 01:27:24 by riblanc           #+#    #+#             */
/*   Updated: 2020/05/13 18:01:57 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"
#include "ft_printf.h"
#include "minishell.h"
#include <stdlib.h>
#include <dirent.h>
#include <stddef.h>
#include <unistd.h>

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

char	**get_nmatch(char *path, char *file)
{
	struct dirent	*ep;
	char			**lst;
	DIR				*dp;
	int				ret;
	int				i;

	ret = 0;
	dp = opendir(path);
	if (dp != NULL)
	{
		while ((ep = readdir(dp)))
			if (match(ep->d_name, file) && strcmp(".", ep->d_name) &&
					strcmp("..", ep->d_name))
				++ret;
		closedir(dp);
	}
	if (!(lst = malloc(sizeof(char *) * (ret + 2))))
		return (NULL);
	lst[ret] = ft_strndup(file, ft_strlen(file) - 1);
	lst[ret + 1] = 0;
	dp = opendir(path);
	i = 0;
	if (dp != NULL)
	{
		while ((ep = readdir(dp)))
			if (match(ep->d_name, file) && strcmp(".", ep->d_name) &&
					strcmp("..", ep->d_name))
			{
				if (ep->d_type == DT_DIR)
					lst[i] = ft_strjoin(ep->d_name, "/");
				else
					lst[i] = ft_strdup(ep->d_name);
				++i;
			}
		closedir(dp);
	}
	return (lst);
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

void	refresh_completion(t_line *line, char **paths, char *elem)
{
	t_lst_in	*tmp;
	int			i;

	tmp = get_elem_by_pos(line->lst_input, line->pos - 1)->next;
	i = 0;
	i = 0;
	while (tmp && tmp->c == paths[1][i])
	{
		++i;
		++line->pos;
		tmp = tmp->next;
	}
	get_rest_word(line, paths[1], elem);
}

void	append_completion(t_line *line)
{
	int		i;
	int		s_str;
	int		eow;

	s_str = ft_strlen(line->complete.str);
	eow = 0;
	i = 0;
	while (--s_str >= 0)
	{
		if (!s_str && line->complete.str[i] == '/')
			eow = 1;
		if (!(eow && line->buff[0] == '/'))
		{
			add_after(line->lst_input, line->complete.str[i],
				line->complete.pos + i);
			++i;
			++line->pos;
		}
	}
	if (!eow && line->buff[0] != ' ' && line->buff[0] != 10)
	{
		add_after(line->lst_input, ' ', line->complete.pos + i);
		++line->pos;
	}
}

void	auto_complete(t_line *line)
{
	t_lst_in	*lstmp;
	char		**paths;
	char		**lst;
	char		*current_word;
	char		*tmp;
	int			nb_elem;
	int			i;
	int			ret;

	ret = 1;
	current_word = get_current_word(line);
	paths = split_path(current_word);
	tmp = ft_strjoin(paths[1], "*");
	free(paths[1]);
	paths[1] = tmp;
	line->complete.str = 0;
	line->complete.pos = -1;
	lstmp = get_elem_by_pos(line->lst_input, line->pos - 1)->next;
	i = 0;
	while (lstmp && lstmp->c == paths[0][i])
	{
		++line->pos;
		++i;
		lstmp = lstmp->next;
	}
	if ((lst = get_nmatch(paths[0], paths[1])))
	{
		nb_elem = ft_strslen(lst);
		i = 0;
		while (line->buff[0] == 9)
		{
			if (ret)
			{
				refresh_completion(line, paths, lst[i % nb_elem]);
				++i;
			}
			ret = read(0, line->buff, 1);
		}
		if (!ft_isalnum(line->buff[0]) && line->buff[0] != 8 &&
				line->buff[0] != 127)
			append_completion(line);
		i = -1;
		while (lst[++i])
			free(lst[i]);
		free(lst);
	}
	free(line->complete.str);
	line->complete.str = NULL;
	line->complete.pos = -1;
	free(paths[0]);
	free(paths[1]);
	free(paths);
	if (current_word)
		free(current_word);
}
