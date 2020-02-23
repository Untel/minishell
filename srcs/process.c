/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 21:38:13 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/23 17:26:34 by adda-sil         ###   ########.fr       */
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
		run_redirect_in(sh, cmd);
		ret = fn(sh, cmd);
		exit(ret);
		return (ret);
	}
	return (SUC);
}

int
	run_redirect_in(t_shell *sh, t_cmd *cmd)
{
	t_redirect	*red;
	t_list		*lst;
	int			fd;
	int			ret;
	char		buff[BUFFER_SIZE + 1];

	if (!cmd->redir_in && !cmd->left)
		return (SUC);
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
	return (SUC);
}

int
	run_redirect_out(t_shell *sh, t_cmd *cmd)
{
	t_redirect	*red;
	t_list		*lst;
	int			fd;

	fd = 0;
	if ((lst = cmd->redir_out))
	{
		while (lst && (red = (t_redirect *)lst->content))
		{
			fd = open(red->filename, O_WRONLY | O_CREAT |
				(red->type == OUT_END_REDIR ? O_APPEND : 0)
				, 0644);
			lst = lst->next;
			if (lst)
				close(fd);
		}
		if (fd > 2)
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
	else if (cmd->right)
		dup2(cmd->pipe[PIPE_IN], STDOUT_FILENO);
	return (SUC);
}

int
	after_redirect_out(t_shell *sh, t_cmd *cmd)
{
	int			fd;
	int			ret;
	char		buff[BUFFER_SIZE + 1];

	fd = open(((t_redirect *)(ft_lstlast(cmd->redir_out)->content))->filename, O_RDONLY);
	while ((ret = read(fd, buff, BUFFER_SIZE)) > 0)
		write(cmd->pipe[PIPE_IN], buff, ret);
	close(fd);
}
