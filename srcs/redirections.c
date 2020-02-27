/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 17:33:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/27 14:10:34 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	redirect_buffer(int from, int to)
{
	int		ret;
	char	buff[BUFFER_SIZE + 1];

	while ((ret = read(from, &buff, BUFFER_SIZE)))
		write(to, buff, ret);
	return (SUC);
}

void
	run_redirect_in(t_shell *sh, t_cmd *cmd)
{
	t_redirect	*red;
	t_list		*lst;
	int			fd;
	int			p[2];

	if (pipe(p) == ERR)
		err_shutdown(sh, "Cannot mount pipe redir in pipe");
	if (cmd->left)
		redirect_buffer(cmd->left->pipe[PIPE_OUT], p[PIPE_IN]);
	if ((lst = cmd->redir_in))
		while (lst && (red = (t_redirect *)lst->content))
		{
			if (red->type == IN_REDIR
				&& (fd = open(red->filename, O_RDONLY)) > 2
				&& redirect_buffer(fd, p[PIPE_IN]))
				close(fd);
			else if (red->type == HEREDOC)
				write(p[PIPE_IN],
					red->value, ft_strlen(red->value));
			lst = lst->next;
		}
	close(p[PIPE_IN]);
	dup2(p[PIPE_OUT], STDIN_FILENO);
	close(p[PIPE_OUT]);
}

int
	run_redirect_out(t_shell *sh, t_cmd *cmd)
{
	t_redirect	*red;
	t_list		*lst;
	int			fd;

	fd = -1;
	if ((lst = cmd->redir_out))
	{
		while (lst && (red = (t_redirect *)lst->content))
		{
			fd = open(red->filename, O_WRONLY | O_CREAT |
				(red->type == OUT_END_REDIR ? O_APPEND : O_TRUNC)
				, 0644);
			lst = lst->next;
			if (red->fd != STDOUT_FILENO)
				dup2(fd, red->fd);
			// if (lst)
			// 	close(fd);
		}
		if (fd != ERR && dup2(fd, STDOUT_FILENO) != ERR)
			close(fd);
	}
	else if (cmd->right)
		dup2(cmd->pipe[PIPE_IN], STDOUT_FILENO);
	return (SUC);
}

int
	after_redirect_out(t_shell *sh, t_cmd *cmd)
{
	int			fd;
	t_list		*lst;
	t_redirect	*red;

	if (!(lst = ft_lstlast(cmd->redir_out)))
		return (FALSE);
	if (!(red = (t_redirect *)(lst->content)))
		return (FALSE);
	if ((fd = open(red->filename, O_RDONLY)) == -1)
		return (FALSE);
	redirect_buffer(fd, cmd->pipe[PIPE_IN]);
	close(fd);
	return (SUC);
}
