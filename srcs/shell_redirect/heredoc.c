/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 18:43:20 by adda-sil          #+#    #+#             */
/*   Updated: 2020/04/01 16:23:37 by khaase           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	add_heredoc(t_shell *sh, int *i)
{
	t_heredoc	*hd;
	size_t		j;

	j = 0;
	if (!(hd = malloc(sizeof(t_heredoc))))
		return (ERR);
	while (sh->input[*i] && sh->input[*i + 1] == ' ')
		*i = *i + 1;
	*hd = (t_heredoc) { .label = NULL, .buffer = NULL, };
	while (sh->input[*i + j] && ft_isalnum(sh->input[*i + 1 + j]))
		j++;
	if (j == 0)
		return (ft_fprintf(STDERR, MSG_ERROR, "heredoc has no label") && 0);
	hd->label = ft_strndup(&sh->input[*i + 1], j);
	*i += j;
	ft_lstadd_back(&sh->heredocs, ft_lstnew(hd, sizeof(t_heredoc *)));
	return (SUC);
}

int
	ask_heredocs(t_shell *sh)
{
	t_heredoc	*hd;
	t_list		*lst;
	char		asker[BUFFER_SIZE];
	char		*tmp;

	lst = sh->heredocs;
	tmp = NULL;
	while (lst)
	{
		hd = (t_heredoc *)lst->content;
		ft_sprintf(asker, "heredoc(%s)", hd->label);
		while (ask_concat(sh, asker, &tmp, hd->label) == SUC)
			;
		hd->buffer = ft_strjoin(tmp, "\n");
		ft_memdel((void **)&tmp);
		lst = lst->next;
	}
	return (SUC);
}
