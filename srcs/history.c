/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 14:28:24 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/24 18:59:50 by adda-sil         ###   ########.fr       */
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
	sh->history.elements = NULL;
	sh->history.index = NULL;
	if ((fd = open(sh->history.path, O_RDONLY)) == -1)
		return (SUC);
	while ((ret = get_next_line(fd, &str[0])) > 0)
		if (ft_strlen(str[0]))
			ft_bilstadd_front(&(sh->history.elements),
				ft_bilstnew(str[0], ft_strlen(str[0])));
	if (ret == 0 && (get_next_line(fd, &str[0]) != -1) && ft_strlen(str[0]))
		ft_bilstadd_front(&(sh->history.elements),
			ft_bilstnew(str[0], ft_strlen(str[0])));
	close(fd);
}

int
	add_to_history(t_shell *sh)
{
	// ft_lstremove_if((t_bilist **)&(sh->history.elements), sh->input, ft_strcmp, free);
	// if (ft_lstsize(sh->history.elements) >= 100)
	// 	ft_lstdelone(ft_lstpop(&(sh->history.elements)), free_history);
	if (ft_strlen(sh->input) > 0)
		ft_bilstadd_front(&(sh->history.elements),
			ft_bilstnew(sh->input, ft_strlen(sh->input)));
	ft_lstprint((t_list *)sh->history.elements, "New history is");
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
	close(fd);
}