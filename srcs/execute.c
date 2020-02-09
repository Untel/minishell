/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:24:06 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/09 17:05:42 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
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
		ft_strlen(cmd->argv[0])) == 0);
}

int
	exec_line(t_shell *sh, t_cmd *cmd)
{
	int	tmp;

	if (cmd->argc == 0)
		return (1);
	if (exec_is(cmd, BUILTIN_EXIT))
		(sh->stop = 1);
	else if (exec_is(cmd, BUILTIN_EXPORT))
		sh->last_ret = export_env(sh, cmd);
	else if (exec_is(cmd, BUILTIN_UNSET))
		sh->last_ret = unset_env(sh, cmd);
	else if (exec_is(cmd, BUILTIN_ENV))
		sh->last_ret = ft_env(sh->env, cmd->argc);
	else if (exec_is(cmd, BUILTIN_CD))
		sh->last_ret = change_directory(sh, cmd);
	else if (exec_bin(sh, cmd))
		;
	else if (test_dir("./", cmd->argv[0]) && cmd->argc == 1)
		sh->last_ret = change_directory(sh, cmd);
	else
		ft_fprintf(2, MSG_404_CMD, cmd->argv[0]) && (sh->last_ret = 127);
	return (1);
}

int
	print_command(t_shell *sh, t_cmd *cmd)
{
	int		i;
	char	*str;
	int		size;

	i = -1;
	str = NULL;
	if (cmd->argc == 0)
		printf("/!\\ Commands without args\n");
	else
	{
		size = cmd->argc - 1;
		if (size)
			str = ft_strmjoin(cmd->argc - 1, &cmd->argv[1], ", ");
		printf("Executing '%s' with %d args: '%s' | LR %d | OP %d\n",
				cmd->argv[0],
			size, str ? str : "", sh->last_ret, cmd->op);
	}
}

int
	exec_lines(t_shell *sh)
{
	t_cmd	*cmd;
	t_list	*lst;
	int		prev_fd;

	lst = sh->cmds;
	while (lst)
	{
		cmd = (t_cmd *)lst->content;
		if (!(cmd->op == OR && sh->last_ret == EXIT_SUCCESS)
			&& !(cmd->op == AND && sh->last_ret != EXIT_SUCCESS))
			exec_line(sh, cmd);
		lst = lst->next;
	}
	return (SUC);
}
