/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 21:38:13 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/23 18:17:36 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	builtin_subprocess(t_shell *sh, t_cmd *cmd, int (*fn)(t_shell *sh, t_cmd *cmd))
{
	pid_t 	child;
	pid_t 	pid;
	int 	status;
	int 	ret;

	child = fork();
	if (child == -1)
		return (-1);
	if (child > 0)
	{
		pid = waitpid(child, &status, 0);
		if (cmd->redir_out && cmd->right)
			after_redirect_out(sh, cmd);
		if (cmd->right)
			close(cmd->pipe[PIPE_IN]);
		if (cmd->left)
			close(cmd->left->pipe[PIPE_OUT]);
		return (status);
	}
	else
	{
		run_redirect_out(sh, cmd);
		if (cmd->redir_in || cmd->left)
			run_redirect_in(sh, cmd);
		ret = fn(sh, cmd);
		exit(ret);
		return (ret);
	}
	return (SUC);
}

int
	exec_cmd(t_shell *sh, t_cmd *cmd, int (*fn)(t_shell *sh, t_cmd *cmd))
{
	if (cmd->right || cmd->left)
		return (builtin_subprocess(sh, cmd, fn));
	else
		return (fn(sh, cmd));
}

int
	mount_pipes(t_shell *sh)
{
	t_cmd	*cmd;
	t_list	*lst;

	lst = sh->cmds;
	while (lst)
	{
		cmd = (t_cmd *)lst->content;
		if (pipe(cmd->pipe) == ERR)
			return (-1);
		lst = lst->next;
	}
	return (1);
}
