/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 15:38:03 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/10 18:34:24 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	export_env(t_shell *sh, t_cmd *cmd)
{
	char	*pos;

	if (cmd->argc > 1)
	{
		pos = ft_strchr(cmd->argv[1], '=');
		if (!pos)
			return (0);
		*pos++ = 0;
		if (!set_value(&sh->env, cmd->argv[1], pos))
			ft_fprintf(2, MSG_ERROR, "export: not an identifier: 8ab");
	}
	else
		return (ft_env(sh->env, 1));
	return (0);
}

int
	unset_env(t_shell *sh, t_cmd *cmd)
{
	int i;

	if (cmd->argc <= 1)
	{
		ft_fprintf(2, MSG_ERROR, "unset: not enough arguments");
		return (0);
	}
	i = 0;
	while (++i < cmd->argc)
		unset_key(&sh->env, cmd->argv[i]);
	return (0);
}

int
	builtin_subprocess(t_shell *sh, t_cmd *cmd, int (*fn)(t_shell *sh, t_cmd *cmd))
{
	pid_t 	child;
	pid_t 	pid;
	int 	status;
	int 	ret;

	child = fork();
	if (child == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_DFL);
	}
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
	tcsetattr(1, 0, &sh->old_term);
	signal(SIGINT, sigint_quit);
	tcsetattr(1, 0, &sh->term);
	return (1);
}
