/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:24:06 by adda-sil          #+#    #+#             */
/*   Updated: 2021/05/13 01:56:39 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#define BUILTIN_EXIT "exit"
#define BUILTIN_EXPORT "export"
#define BUILTIN_UNSET "unset"
#define BUILTIN_ENV "env"
#define BUILTIN_CD "cd"
#define BUILTIN_ECHO "echo"
#define BUILTIN_PWD "pwd"

int
	exec_is(t_cmd *cmd, char *str)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(cmd->argv[0]))
	{
		cmd->argv[0][i] = ft_tolower(cmd->argv[0][i]);
		i++;
	}
	return (ft_strcmp(str, cmd->argv[0]) == 0);
}

int
	say_404(t_shell *sh, t_cmd *cmd)
{
	(void)sh;
	ft_fprintf(STDERR, MSG_404_CMD, cmd->argv[0]);
	return (127);
}

int
	exec_line(t_shell *sh, t_cmd *cmd)
{
	if (cmd->argc == 0)
		return (1);
	else if (exec_is(cmd, BUILTIN_EXIT))
		sh->last_ret = exec_cmd(sh, cmd, exit_prog);
	else if (exec_is(cmd, BUILTIN_ECHO))
		sh->last_ret = exec_cmd(sh, cmd, ft_echo);
	else if (exec_is(cmd, BUILTIN_PWD))
		sh->last_ret = exec_cmd(sh, cmd, ft_pwd);
	else if (exec_is(cmd, BUILTIN_EXPORT))
		sh->last_ret = exec_cmd(sh, cmd, export_env);
	else if (exec_is(cmd, BUILTIN_UNSET))
		sh->last_ret = exec_cmd(sh, cmd, unset_env);
	else if (exec_is(cmd, BUILTIN_ENV))
		sh->last_ret = exec_cmd(sh, cmd, ft_env);
	else if (exec_is(cmd, BUILTIN_CD))
		sh->last_ret = exec_cmd(sh, cmd, change_directory);
	else if (exec_bin(sh, cmd))
		;
	else
		sh->last_ret = exec_cmd(sh, cmd, say_404);
	return (SUC);
}

int
	exec_lines(t_shell *sh)
{
	t_cmd	*cmd;
	t_list	*lst;

	if (mount_pipes(sh) == ERR)
		return (ft_fprintf(STDERR, MSG_ERROR, "cannot mount pipes"));
	cmd = NULL;
	lst = sh->cmds;
	while (lst)
	{
		cmd = (t_cmd *)lst->content;
		if (cmd->argc > 0 && ft_strlen(cmd->argv[0]) > 0 &&
			!(cmd->op == OR && sh->last_ret == EXIT_SUCCESS)
			&& !(cmd->op == AND && sh->last_ret != EXIT_SUCCESS))
		{
			if (cmd->argc > 0)
				check_aliases(sh, cmd);
			unset_key(&sh->env, "_");
			set_value(&sh->env, "_", cmd->argv[cmd->argc - 1]);
			exec_line(sh, cmd);
		}
		lst = lst->next;
	}
	ft_lstclear(&sh->cmds, free_command);
	return (SUC);
}
