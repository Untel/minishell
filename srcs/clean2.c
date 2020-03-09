/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 17:17:17 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/09 17:17:37 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	free_command(t_list *lst)
{
	t_cmd *cmd;

	cmd = (t_cmd *)lst->content;
	while (cmd->argc--)
		ft_memdel((void **)&cmd->argv[cmd->argc]);
	ft_lstclear(&cmd->redir_in, free_redirection);
	ft_lstclear(&cmd->redir_out, free_redirection);
	ft_memdel((void **)&cmd->argv);
	ft_memdel((void **)&cmd);
}
