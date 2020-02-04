/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:24:06 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 20:08:47 by adda-sil         ###   ########.fr       */
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
	int	tmp;

	ret = 0;
	if (cmd->argc == 0)
		return (1);
	//	err_shutdown(sh, "Erreur aucun arguments");
	if (exec_is(cmd, BUILTIN_EXIT))
		(sh->stop = 1) &&
		(ret = 1);
	else if (exec_is(cmd, BUILTIN_EXPORT))
		ret = export_env(sh, cmd);
	else if (exec_is(cmd, BUILTIN_UNSET))
		ret = unset_env(sh, cmd);
	else if (exec_is(cmd, BUILTIN_ENV))
		ret = ft_env(sh->env, cmd->argc);
	else if (exec_is(cmd, BUILTIN_CD))
		ret = change_directory(sh, cmd);
	else if ((tmp = exec_bin(sh->env, cmd)))
		ret = tmp;
	else if (0) //try to cd, if ret falsem say msg
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
