/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/02 20:14:09 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 19:26:48 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd
	*new_command(t_shell *sh)
{
	t_cmd	*cmd;
	t_list	*el;

	if (!(cmd = malloc(sizeof(t_cmd))))
		return (NULL);
	*cmd = (t_cmd) { .input = NULL, .argc = 0, .argv = NULL };
	if (!(el = ft_lstnew(cmd, sizeof(t_cmd *))))
	{
		free(cmd);
		return (NULL);
	}
	ft_lstadd_back(&(sh->cmds), el);
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

char
	*add_arg_to_last_cmd(t_shell *sh, char *str)
{
	t_list *el;

	el = ft_lstlast(sh->cmds);
	if (!el)
		err_shutdown(sh, "Error commands.c add_arg_to_last_cmd");
	return (add_argument((t_cmd *)el->content, str));
}

void
	free_command(t_list *lst)
{
	t_cmd *cmd;

	cmd = (t_cmd *)lst->content;
	while (cmd->argc--)
	{
		free(cmd->argv[cmd->argc]);
		cmd->argv[cmd->argc] = NULL;
	}
	free(cmd->argv);
	cmd->argv = NULL;
	free(cmd);
	cmd = NULL;
}
