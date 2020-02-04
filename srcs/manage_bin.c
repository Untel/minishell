/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_bin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 01:56:11 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/04 09:10:52 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <dirent.h>

int		try_exec(char *path, char **argv, char **envp, pid_t child)
{
	int		status;
	int		ret;
	pid_t	pid;

	if (child == -1)
		return (-1);
	if (child > 0)
	{
		pid = waitpid(child, &status, 0);
		return (status);
	}
	else
	{
		ret = execve(path, argv, envp);
		return (ret);
	}
	return (-1);
}

int		test_dir(char *path, char *cmd)
{
	struct dirent	*file;
	DIR				*rep;

	file = NULL;
	rep = NULL;
	if ((rep = opendir(path)) == NULL)
		return (-1);
	while ((file = readdir(rep)) != NULL)
	{
		if (!ft_strncmp(file->d_name, cmd, ft_strlen(cmd) + 1))
		{
			if (closedir(rep) == -1)
				return (-1);
			return (1);
		}
	}
	if (closedir(rep) == -1)
		return (-1);
	return (0);
}

int		fork_exec(t_list *lst_env, t_cmd *cmd, char *tmp[2])
{
	pid_t	child;
	char	**envp;
	char	*bin_path;
	char	*res;
	int		ret;

	child = -1;
	envp = convert_env_list(lst_env);
	child = fork();
	bin_path = ft_strmjoin(2, tmp, "/");
	ret = try_exec(bin_path, cmd->argv, envp, child);
	if (child == 0 && ret != 0)
	{
		exit(ret);
		return (ret);
	}
	free(bin_path);
	free_env_array(envp);
	return (0);
}

int		exec_bin(t_list *lst_env, t_cmd *cmd)
{
	char	**paths;
	char	*tmp[2];
	int		i;
	int		ret;

	paths = ft_split(get_value(lst_env, "PATH"), ':');
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
		return (0);
	}
	fork_exec(lst_env, cmd, tmp);
	free_env_array(paths);
	return (1);
}
