/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_complete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 01:27:24 by riblanc           #+#    #+#             */
/*   Updated: 2020/10/13 01:28:59 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edit.h"
#include "minishell.h"
#include <stdlib.h>
#include <dirent.h>
#include <stddef.h>
#include <unistd.h>

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

void	auto_complete_init(t_cplutils *utils, t_line *line)
{
	utils->ret = 1;
	utils->current_word = get_current_word(line);
	utils->paths = split_path(utils->current_word);
	utils->tmp = ft_strjoin(utils->paths[1], "*");
	free(utils->paths[1]);
	utils->paths[1] = utils->tmp;
	line->complete.str = 0;
	line->complete.pos = -1;
	utils->lstmp = get_elem_by_pos(line->lst_input, line->pos - 1)->next;
	utils->i = 0;
	while (utils->lstmp && utils->lstmp->c == utils->paths[0][utils->i])
	{
		if (utils->i == 0 && utils->lstmp->c == '.'
				&& utils->paths[1][utils->i] == '.'
				&& utils->lstmp->next && utils->lstmp->next->c != '/')
			break ;
		++line->pos;
		++utils->i;
		utils->lstmp = utils->lstmp->next;
	}
}

void	refresh_auto_complete(t_cplutils *u, t_line *line)
{
	u->i = 0;
	if (u->nb_elem > 2)
		while (line->buff[0] == 9)
		{
			if (u->ret)
				refresh_completion(line, u->paths, u->lst[u->i++ % u->nb_elem]);
			u->ret = read(0, line->buff, 1);
		}
	else
		refresh_completion(line, u->paths, u->lst[0]);
	if (!ft_isalnum(line->buff[0]) && line->buff[0] != 8 &&
			line->buff[0] != 127)
		append_completion(line);
	if (line->buff[0] == 127 || line->buff[0] == 8)
		line->buff[0] = 0;
	u->i = -1;
	while (u->lst[++u->i])
		free(u->lst[u->i]);
	free(u->lst);
}

void	auto_complete(t_shell *sh, t_line *line)
{
	t_cplutils	utils;

	auto_complete_init(&utils, line);
	if ((utils.lst = get_nmatch(sh, &utils.paths[0], utils.paths[1])))
	{
		utils.nb_elem = ft_strslen(utils.lst);
		refresh_auto_complete(&utils, line);
	}
	free(line->complete.str);
	line->complete.str = NULL;
	line->complete.pos = -1;
	free(utils.paths[0]);
	free(utils.paths[1]);
	free(utils.paths);
	if (utils.current_word)
		free(utils.current_word);
}
