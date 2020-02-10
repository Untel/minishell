/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 21:38:13 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/10 21:38:48 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	builtin_subprocess(t_shell *sh, t_cmd *cmd,
		int (*fn)(t_shell *sh, t_cmd *cmd))
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
		if (cmd->right)
			close(cmd->pipe[PIPE_IN]);
		if (cmd->left)
			close(cmd->left->pipe[PIPE_OUT]);
		return (status);
	}
	else
	{
		if (cmd->right)
			dup2(cmd->pipe[PIPE_IN], STDOUT);
		if (cmd->left)
			dup2(cmd->left->pipe[PIPE_OUT], STDIN);
		ret = fn(sh, cmd);
		exit(ret != 0);
		return (ret);
	}
	return (1);
}