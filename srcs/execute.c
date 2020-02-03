/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:24:06 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 21:19:19 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define BUILTIN_EXIT "exit"
#define BUILTIN_EXPORT "export"
#define BUILTIN_UNSET "unset"
#define BUILTIN_ENV "env"
#define BUILTIN_CD "cd"

int
	exec_is(t_cmd *cmd, char *str)
{
	return (ft_strncmp(
		str,
		cmd->argv[0],
		ft_strlen(cmd->argv[0])
	) == 0);
}

int
	exec_line(t_shell *sh, t_cmd *cmd)
{
	int ret;

	ret = 0;
	if (cmd->argc == 0)
		return (1);
	// 	err_shutdown(sh, "Erreur aucun arguments");
	if (exec_is(cmd, BUILTIN_EXIT))
		(sh->stop = 1) &&
		(ret = 1);
	else if (exec_is(cmd, BUILTIN_EXPORT))
		;// TODO call the function as fn(cmd->argc - 1, &cmd->argv[1])
	else if (exec_is(cmd, BUILTIN_UNSET))
		;
	else if (exec_is(cmd, BUILTIN_ENV))
		;
	else if (exec_is(cmd, BUILTIN_CD))
		ret = change_directory(sh, cmd);
	else if (0) //Find executable cmd->argv[0] in all dir defined in $PATH
		;
	else
		ft_printf(MSG_404_CMD, cmd->argv[0]) && (ret = 1);
	return (ret);
}

int
	exec_lines(t_shell *sh)
{
	t_cmd	*cmd;
	t_list	*lst;

	lst = sh->cmds;
	while (lst)
	{
		cmd = (t_cmd *)lst->content;
		exec_line(sh, cmd);
		lst = lst->next;
	}
	return (SUC);
}