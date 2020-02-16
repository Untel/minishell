/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:24:06 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/16 18:34:07 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#define BUILTIN_EXIT "exit"
#define BUILTIN_EXPORT "export"
#define BUILTIN_UNSET "unset"
#define BUILTIN_ENV "env"
#define BUILTIN_CD "cd"

int
	exec_is(t_cmd *cmd, char *str)
{
	return (ft_strncmp(
		str,
		cmd->argv[0],
		ft_strlen(cmd->argv[0])) == 0);
}

int
	redirect_out(t_shell *sh, t_cmd *cmd)
{
	int	fd;
	char buff[BUFFER_SIZE + 1];
	int ret;

	fd = open(cmd->argv[0], O_WRONLY | O_CREAT |
		(cmd->op == REDIR_OUT_END ? O_APPEND : 0)
		, 0644);

	while ((ret = read(0, &buff, BUFFER_SIZE)))
	{
		if (!cmd->right || cmd->right->op == PIPE)
			write(fd, buff, ret);
		if (cmd->right)
			write(STDOUT, buff, ret);
	}
	close(fd);
}

int
	say_404(t_shell *sh, t_cmd *cmd)
{
	ft_fprintf(STDERR, MSG_404_CMD, cmd->argv[0]);
	sh->last_ret = 127;
	return (127);
}

int
	exec_cmd(t_shell *sh, t_cmd *cmd, int (*fn)(t_shell *sh, t_cmd *cmd))
{
	if (cmd->right || cmd->left)
		return (builtin_subprocess(sh, cmd, fn));
	else
		return (fn(sh, cmd));
}

int
	exec_line(t_shell *sh, t_cmd *cmd)
{
	int	tmp;

	if (cmd->argc == 0)
		return (1);
	else if (exec_is(cmd, BUILTIN_EXIT))
		(sh->stop = 1);
	else if (exec_is(cmd, BUILTIN_EXPORT))
		sh->last_ret = exec_cmd(sh, cmd, export_env);
	else if (exec_is(cmd, BUILTIN_UNSET))
		sh->last_ret = exec_cmd(sh, cmd, unset_env);
	else if (exec_is(cmd, BUILTIN_ENV))
		sh->last_ret = exec_cmd(sh, cmd, ft_env);
	else if (exec_is(cmd, BUILTIN_CD))
		sh->last_ret = exec_cmd(sh, cmd, change_directory);
	else if (exec_bin(sh, cmd))
		;
	else
		sh->last_ret = exec_cmd(sh, cmd, say_404);
	return (1);
}

int
	print_command(t_shell *sh, t_cmd *cmd)
{
	int		i;
	char	*str;
	int		size;
	t_list	*lst;

	i = -1;
	str = NULL;
	if (cmd->argc == 0)
		ft_printf("/!\\ Commands without args\n");
	else
	{
		size = cmd->argc - 1;
		if (size)
			str = ft_strmjoin(cmd->argc - 1, &cmd->argv[1], ", ");
		ft_printf("Executing '%s' with %d args: '%s' | LR %d | OP %d\n", cmd->argv[0],
			size, str ? str : "", sh->last_ret, cmd->op);
		if ((lst = cmd->redir_in))
			while (lst)
			{
				ft_printf("Redir input arg = %s label = %s\n", ((t_redirect *)lst->content)->filename, ((t_redirect *)lst->content)->value);
				lst = lst->next;
			}
		else
			ft_printf("No redir in\n");
	}
}

int
	mount_pipes(t_shell *sh)
{
	t_cmd	*cmd;
	t_list	*lst;

	lst = sh->cmds;
	while (lst)
	{
		cmd = (t_cmd *)lst->content;
		if (cmd->right && pipe(cmd->pipe) == ERR)
			return (-1);
		lst = lst->next;
	}
	return (1);
}

int
	exec_lines(t_shell *sh)
{
	t_cmd	*cmd;
	t_list	*lst;

	if (mount_pipes(sh) == ERR)
		return (ft_fprintf(STDERR, MSG_ERROR, "cannot mount pipes"));
	lst = sh->cmds;
	while (lst)
	{
		cmd = (t_cmd *)lst->content;
		if (cmd->op == REDIR_OUT_END || cmd->op == REDIR_OUT)
			builtin_subprocess(sh, cmd, redirect_out);
		else if (!(cmd->op == OR && sh->last_ret == EXIT_SUCCESS)
			&& !(cmd->op == AND && sh->last_ret != EXIT_SUCCESS))
			exec_line(sh, cmd);
		lst = lst->next;
	}
	return (SUC);
}
