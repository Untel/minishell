/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:21:24 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/23 17:40:23 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	clear_last_prompt(t_shell *sh)
{
	ft_lstclear(&sh->heredocs, free_heredocs);
	ft_lstclear(&sh->cmds, free_command);
	sh->hd_index = 0;
	ft_memdel((void **)&sh->input);
}

void
	err_shutdown(t_shell *sh, char *str)
{
	ft_fprintf(STDERR, MSG_ERROR, str);
	clear_last_prompt(sh);
	exit(1);
}

void
	free_heredocs(t_list *lst)
{
	t_heredoc *hd;

	hd = (t_heredoc *)lst->content;
	ft_memdel((void **)&hd->label);
	ft_memdel((void **)&hd->buffer);
	ft_memdel((void **)&hd);
}
