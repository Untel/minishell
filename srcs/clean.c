/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:21:24 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/25 22:40:12 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	clear_last_prompt(t_shell *sh)
{
	ft_lstclear(&sh->heredocs, free_heredocs);
	ft_lstclear(&sh->cmds, free_command);
	sh->hd_index = 0;
}

void
	err_shutdown(t_shell *sh, char *str)
{
	ft_fprintf(STDERR, MSG_ERROR, str);
	clear_last_prompt(sh);
	exit(EXIT_SUCCESS);
}

void
	free_heredocs(t_list *lst)
{
	t_heredoc *hd;

	hd = (t_heredoc *)lst->content;
	ft_memdel((void **)&hd->label);
	ft_memdel((void **)&hd->buffer);
	// ft_memdel((void **)&hd);
}

void
	free_content(t_list *element)
{
	ft_memdel((void **)element->content);
}

void
	free_redirection(t_list *lst)
{
	t_redirect	*red;

	red = ((t_redirect *)lst->content);
	if (red->type != HEREDOC)
		ft_memdel((void **)&red->value);
	ft_memdel((void **)&red->filename);
	ft_memdel((void **)&red);
}

int
	reset_history_position(t_shell *sh)
{
	ft_memdel((void **)&sh->history.input);
	sh->history.index = NULL;
	return (SUC);
}
