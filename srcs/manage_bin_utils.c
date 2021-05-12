/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_bin_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/24 19:48:08 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/12 23:20:36 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int		fe_init(t_shell *sh, t_cmd *cmd, char **paths)
{
	char	*tmp[2];
	int		ret;
	int		i;

	ret = 0;
	tmp[1] = cmd->argv[0];
	i = -1;
	while (paths[++i])
	{
		tmp[0] = paths[i];
		ret = test_dir(tmp[0], tmp[1]);
		if (ret == 1)
			break ;
	}
	if (ret != 1)
	{
		free_env_array(paths);
		return (1);
	}
	ret = fork_exec(sh, cmd, tmp, 2);
	return (0);
}

int		check_if_executable(char *path, char *cmd)
{
	char		*tmp[2];
	char		*filename;
	struct stat	statbuf;

	tmp[0] = path;
	tmp[1] = cmd;
	filename = ft_strmjoin(2, tmp, "/");
	if (!filename)
		return (1);
	stat(filename, &statbuf);
	if (statbuf.st_mode & S_IFLNK)
		lstat(filename, &statbuf);
	free(filename);
	return (!(((statbuf.st_mode & S_IFREG) == S_IFREG
		|| (statbuf.st_mode & S_IFLNK) == S_IFLNK) &&
		(statbuf.st_mode & S_IXUSR) == S_IXUSR));
}
