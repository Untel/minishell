/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:28:24 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/25 21:40:06 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	init_history(t_shell *sh)
{
	int		fd;
	int		ret;
	char	*str[2];

	str[0] = get_value(sh->env, "HOME", "~");
	str[1] = ".minishell_history";
	sh->history.path = ft_strmjoin(2, (char **)str, "/");
	sh->history.input = NULL;
	sh->history.index = NULL;
	sh->history.elements = NULL;
	if ((fd = open(sh->history.path, O_RDONLY)) == -1)
		return (SUC);
	while ((ret = get_next_line(fd, &str[0])) > 0)
		if (ft_strlen(str[0]))
			ft_bilstadd_front(&(sh->history.elements),
				ft_bilstnew(str[0], ft_strlen(str[0])));
		else
			ft_memdel((void **)&str[0]);
	if (ret == 0)
	{
		if (ft_strlen(str[0]))	
			ft_bilstadd_front(&(sh->history.elements),
				ft_bilstnew(str[0], ft_strlen(str[0])));
		else
			ft_memdel((void **)&str[0]);
	}
	close(fd);
	return (SUC);
}

int
	add_to_history(t_shell *sh)
{
	ft_bilstremove_if(&(sh->history.elements), sh->input, ft_strcmp, free);
	if (ft_strlen(sh->input) > 0)
		ft_bilstadd_front(&(sh->history.elements),
			ft_bilstnew(sh->input, ft_strlen(sh->input)));
	else
	{
		ft_memdel((void **)&sh->input);
		return (FALSE);
	}
	return (SUC);
}


int
	persist_history(t_shell *sh)
{
	int			fd;
	t_bilist	*el;

	if ((fd = open(sh->history.path, O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
		err_shutdown(sh, "Cannot open minishell history");
	while ((el = sh->history.elements))
	{
		write(fd, el->content, el->size);
		sh->history.elements = el->next;
		if (el->next)
			write(fd, "\n", 1);
		ft_memdel((void **)&el->content);
		ft_memdel((void **)&el);
	}
	ft_memdel((void **)&sh->history.path);
	ft_memdel((void **)&sh->history.input);
	close(fd);
	return (SUC);
}

void
	print_history(t_shell *sh, t_term *term, int next)
{
	t_bilist		*idx;
	char			*str;

	if (!sh->history.input)
		sh->history.input = get_current_word(sh);
	str = sh->history.input;
	if (!sh->history.index)
		idx = sh->history.elements;
	else
	{
		if (next)
			idx = (sh->history.index->next ? sh->history.index->next : NULL);
		else
			idx = (sh->history.index->prev ? sh->history.index->prev : NULL);
	}
	while (idx && sh->history.index != idx)
	{
		if (match(idx->content, str))
		{
			sh->history.index = idx;
			add_str_to_lst(sh, str, idx->content);
			return ;
		}
		idx = next ? idx->next : idx->prev;
	}
}

int
	reset_history_position(t_shell *sh)
{
	ft_memdel((void **)&sh->history.input);
	sh->history.index = NULL;
	return (SUC);
}
