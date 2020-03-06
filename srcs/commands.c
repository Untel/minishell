/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 20:14:09 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/06 01:56:37 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd
	*new_command(t_shell *sh, t_operator op)
{
	t_cmd	*cmd;
	t_list	*el;
	t_list	*prev;

	if (!(cmd = malloc(sizeof(t_cmd))))
		return (NULL);
	*cmd = (t_cmd) { .input = NULL, .argc = 0, .argv = NULL, .env = NULL,
		.op = op, .left = NULL, .right = NULL, .pipe = { 0, 0 },
		.redir_in = NULL, .redir_out = NULL };
	if (!(el = ft_lstnew(cmd, sizeof(t_cmd *)))
		&& ft_imemdel((void **)&cmd))
		return (NULL);
	if ((prev = ft_lstlast(sh->cmds)))
	{
		if (op == PIPE)
		{
			cmd->left = prev->content;
			((t_cmd *)prev->content)->right = cmd;
		}
		prev->next = el;
	}
	else
		ft_lstadd_front(&(sh->cmds), el);
	return (cmd);
}

char
	*add_argument(t_cmd *cmd, char *str)
{
	char	**new_argv;
	int		i;

	if (!(new_argv = malloc(sizeof(char *) * (cmd->argc + 2))))
		return (NULL);
	i = -1;
	while (++i < cmd->argc)
		new_argv[i] = cmd->argv[i];
	new_argv[cmd->argc++] = str;
	new_argv[cmd->argc] = NULL;
	ft_memdel((void **)&cmd->argv);
	cmd->argv = new_argv;
	return (str);
}

void
	add_redir(t_shell *sh, t_cmd *cmd, char *str, t_read *rd)
{
	t_redirect	*redir;
	t_list		*lst;
	int			red_dir;

	red_dir = (rd->add_to == HEREDOC || rd->add_to == IN_REDIR);
	if (!(redir = malloc(sizeof(t_redirect))))
		return ;
	*redir = (t_redirect) { .filename = str, .value = NULL, .type = rd->add_to,
		.fd = rd->fd };
	if (red_dir && rd->fd == -1)
		redir->fd = STDIN_FILENO;
	else if (!red_dir && rd->fd == -1)
		redir->fd = STDOUT_FILENO;
	rd->fd = -1;
	if (rd->add_to == HEREDOC &&
		(lst = ft_lstindex(sh->heredocs, sh->hd_index++)))
		redir->value = ((t_heredoc *)lst->content)->buffer;
	ft_lstadd_back(
		(red_dir
			? &cmd->redir_in
			: &cmd->redir_out)
		, ft_lstnew(redir, sizeof(t_redirect)));
}

void
	add_arg_to_last_cmd(t_shell *sh, char *str, t_read *rd)
{
	t_list	*el;

	ft_escape(str, '\\');
	if (!(el = ft_lstlast(sh->cmds)))
		err_shutdown(sh, "Error commands.c add_arg_to_last_cmd");
	if (rd->add_to != ARGS)
		add_redir(sh, (t_cmd *)el->content, str, rd);
	else
		add_argument((t_cmd *)el->content, str);
	rd->add_to = ARGS;
}

void
	free_command(t_list *lst)
{
	t_cmd *cmd;

	cmd = (t_cmd *)lst->content;
	while (cmd->argc--)
		ft_memdel((void **)&cmd->argv[cmd->argc]);
	ft_lstclear(&cmd->redir_in, free_redirection);
	ft_lstclear(&cmd->redir_out, free_redirection);
	ft_memdel((void **)&cmd->argv);
	ft_memdel((void **)&cmd);
}
