/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 20:14:09 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/16 20:30:52 by adda-sil         ###   ########.fr       */
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
	*cmd = (t_cmd) { .input = NULL, .argc = 0, .argv = NULL,
		.op = op, .left = NULL, .right = NULL, .pipe = NULL,
		.redir_in = NULL, .pipe_redir_in = NULL };
	if (!(el = ft_lstnew(cmd, sizeof(t_cmd *))))
	{
		ft_memdel((void **)&cmd);
		return (NULL);
	}
	if ((prev = ft_lstlast(sh->cmds)))
	{
		if (op == PIPE || op == REDIR_OUT || op == REDIR_OUT_END)
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
	free(cmd->argv);
	cmd->argv = new_argv;
	return (str);
}

void
	add_redir(t_cmd *cmd, char *str, t_read *rd)
{
	t_redirect	*redir;
	t_list		**lst;

	lst = &cmd->redir_in;
	if (!(redir = malloc(sizeof(t_redirect))))
		return ;
	*redir = (t_redirect) { .filename = str, .value = NULL, .type = rd->add_to };
	ft_lstadd_back(lst, ft_lstnew(redir, sizeof(t_redirect)));
}

void
	add_arg_to_last_cmd(t_shell *sh, char *str, t_read *rd)
{
	t_list	*el;
	el = ft_lstlast(sh->cmds);

	if (!el)
		err_shutdown(sh, "Error commands.c add_arg_to_last_cmd");
	if (rd->add_to != ARGS)
		add_redir((t_cmd *)el->content, str, rd);
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
	ft_memdel((void **)&cmd->argv);
	ft_memdel((void **)&cmd);
}
