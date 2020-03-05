/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 17:33:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/05 16:15:45 by adda-sil         ###   ########.fr       */
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
	t_redirect	*redir;
	t_list		*lst;
	int			fd;
	int			p[2];

	if (pipe(p) == ERR)
		err_shutdown(sh, "Cannot mount pipe redir in pipe");
	if (cmd->left)
		redirect_buffer(cmd->left->pipe[PIPE_OUT], p[PIPE_IN]);
	if ((lst = cmd->redir_in))
		while (lst && (redir = (t_redirect *)lst->content))
		{
			if (redir->fd == STDIN_FILENO)
				redir->fd = p[PIPE_IN];
			if (redir->type == IN_REDIR
				&& (fd = open(redir->filename, O_RDONLY)) > 2
				&& redirect_buffer(fd, redir->fd))
				close(fd);
			else if (redir->type == HEREDOC)
				write(redir->fd, redir->value, ft_strlen(redir->value));
			lst = lst->next;
		}
	close(p[PIPE_IN]);
	dup2(p[PIPE_OUT], STDIN_FILENO);
	close(p[PIPE_OUT]);
}

int
	run_redirect_out(t_shell *sh, t_cmd *cmd)
{
	t_redirect	*redir;
	t_list		*lst;
	int			fd;

	fd = -1;
	(void)sh;
	if ((lst = cmd->redir_out))
		while (lst && (redir = (t_redirect *)lst->content))
		{
			if ((fd = open(redir->filename, O_WRONLY | O_CREAT |
				(redir->type == OUT_END_REDIR ? O_APPEND : O_TRUNC)
				, 0644)) != ERR && dup2(fd, redir->fd) != ERR)
				close(fd);
			lst = lst->next;
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

	(void)sh;
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
