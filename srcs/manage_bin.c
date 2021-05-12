/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_bin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 01:56:11 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/12 23:19:10 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>

int
	try_exec(t_shell *sh, char *path, t_cmd *cmd, pid_t child)
{
	int		status;
	int		ret;

	if (child > 0)
	{
		waitpid(child, &status, 0);
		sh->last_ret = WEXITSTATUS(status);
		after_child_exec(sh, cmd);
		return (sh->last_ret);
	}
	else
	{
		run_redirect_out(sh, cmd);
		if (cmd->redir_in || cmd->left)
			run_redirect_in(sh, cmd);
		errno = 0;
		if (cmd->argc > 0)
			set_value(&sh->env, "_", cmd->argv[cmd->argc - 1]);
		ret = execve(path, cmd->argv,
			(cmd->env = convert_env_list(sh->env)));
		if (errno != 0)
			ft_fprintf(STDERR, MSG_ERROR, strerror(errno));
		return (ret);
	}
	return (ERR);
}

int
	test_dir(char *path, char *cmd)
{
	struct dirent	*file;
	DIR				*rep;

	file = NULL;
	rep = NULL;
	if ((rep = opendir(path)) == NULL)
		return (ERR);
	while ((file = readdir(rep)) != NULL)
	{
		if (!ft_strncmp(file->d_name, cmd, ft_strlen(cmd) + 1))
		{
			if (check_if_executable(path, cmd))
				return (FALSE);
			if (closedir(rep) == ERR)
				return (ERR);
			return (SUC);
		}
	}
	if (closedir(rep) == ERR)
		return (ERR);
	return (FALSE);
}

int
	ft_inset(char *str, char c)
{
	int		i;

	i = 0;
	while (*(str + i))
		if (*(str + i++) == c)
			return (--i);
	return (-1);
}

int
	fork_exec(t_shell *sh, t_cmd *cmd, char *tmp[2], int nb)
{
	pid_t	child;
	char	*bin_path;
	int		ret;

	child = -1;
	ret = 0;
	if (nb == 2)
		bin_path = ft_strmjoin(2, tmp, "/");
	else if (nb == 1)
		bin_path = ft_strdup(tmp[0]);
	else
		return (FALSE);
	child = fork();
	init_child_signals(child);
	if (child == -1)
		ret = -1;
	else
		ret = try_exec(sh, bin_path, cmd, child);
	if (child == 0 && ret != 0)
		exit(ret);
	ft_memdel((void **)&bin_path);
	return (ret);
}

int
	exec_bin(t_shell *sh, t_cmd *cmd)
{
	char	**paths;
	char	*tmp[2];
	int		ret;

	paths = ft_split(get_value(sh->env, "PATH", NULL), ':');
	if ((ret = ft_inset(cmd->argv[0], '/') != -1))
	{
		if (ret == 1)
			tmp[0] = cmd->argv[0];
		ret = fork_exec(sh, cmd, tmp, 1);
	}
	else if (paths)
		if (fe_init(sh, cmd, paths))
			return (0);
	if (paths)
		free_env_array(paths);
	return (1);
}
