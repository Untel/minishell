/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 21:38:13 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/19 17:49:00 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

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
		if (cmd->right)
			close(cmd->pipe[PIPE_IN]);
		if (cmd->left)
			close(cmd->left->pipe[PIPE_OUT]);
		return (status);
	}
	else
	{
		if (cmd->right)
			dup2(cmd->pipe[PIPE_IN], STDOUT_FILENO);
		run_redirect_in(sh, cmd);
		ret = fn(sh, cmd);
		exit(0);
		return (ret);
	}
	return (1);
}

int
	run_redirect_in(t_shell *sh, t_cmd *cmd)
{
	t_redirect	*red;
	t_list		*lst;
	int			fd;
	int			ret;
	char		buff[BUFFER_SIZE + 1];
	char		*pos;

	if (!cmd->redir_in && !cmd->left)
		return (1);
	pipe(cmd->pipe_redir_in);
	if (cmd->left)
		while ((ret = read(cmd->left->pipe[PIPE_OUT], &buff, BUFFER_SIZE)))
			write(cmd->pipe_redir_in[PIPE_IN], buff, ret);
	if ((lst = cmd->redir_in))
		while (lst && (red = (t_redirect *)lst->content))
		{
			if (red->type == IN_REDIR &&
				(fd = open(red->filename, O_RDONLY)) > 2)
			{
				while ((ret = read(fd, &buff, BUFFER_SIZE)))
					write(cmd->pipe_redir_in[PIPE_IN], buff, ret);
				close(fd);
			}
			else if (red->type == HEREDOC)
				write(cmd->pipe_redir_in[PIPE_IN], red->value, ft_strlen(red->value));
			lst = lst->next;
		}
	close(cmd->pipe_redir_in[PIPE_IN]);
	dup2(cmd->pipe_redir_in[PIPE_OUT], STDIN_FILENO);
	close(cmd->pipe_redir_in[PIPE_OUT]);
}
