/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 17:33:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/23 23:36:36 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	redirect_buffer(int from, int to)
{
	int		ret;
	char	buff[BUFFER_SIZE + 1];

	while ((ret = read(from, &buff, BUFFER_SIZE)))
		write(to, buff, ret);
}

void
	run_redirect_in(t_shell *sh, t_cmd *cmd)
{
	t_redirect	*red;
	t_list		*lst;
	int			fd;

	if (pipe(cmd->pipe_redir_in) == ERR)
		err_shutdown(sh, "Cannot mount pipe redir in pipe");
	if (cmd->left)
		redirect_buffer(cmd->left->pipe[PIPE_OUT], cmd->pipe_redir_in[PIPE_IN]);
	if ((lst = cmd->redir_in))
		while (lst && (red = (t_redirect *)lst->content))
		{
			if (red->type == IN_REDIR &&
				(fd = open(red->filename, O_RDONLY)) > 2)
			{
				redirect_buffer(fd, cmd->pipe_redir_in[PIPE_IN]);
				close(fd);
			}
			else if (red->type == HEREDOC)
				write(cmd->pipe_redir_in[PIPE_IN],
					red->value, ft_strlen(red->value));
			lst = lst->next;
		}
	close(cmd->pipe_redir_in[PIPE_IN]);
	dup2(cmd->pipe_redir_in[PIPE_OUT], STDIN_FILENO);
	close(cmd->pipe_redir_in[PIPE_OUT]);
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
				(red->type == OUT_END_REDIR ? O_APPEND : 0)
				, 0644);
			lst = lst->next;
			if (lst)
				close(fd);
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
