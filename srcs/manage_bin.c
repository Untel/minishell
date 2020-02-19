/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_bin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/04 01:56:11 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/19 17:48:57 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <dirent.h>

int		try_exec(t_shell *sh, char *path, t_cmd *cmd, char **envp, pid_t child)
{
	int		status;
	int		ret;
	pid_t	pid;

	if (child == -1)
		return (-1);
	if (child > 0)
	{
		pid = waitpid(child, &status, 0);
		sh->last_ret = WEXITSTATUS(status);
		if (cmd->right)
			close(cmd->pipe[PIPE_IN]);
		if (cmd->left)
			close(cmd->left->pipe[PIPE_OUT]);
		return (sh->last_ret);
	}
	else
	{
		if (cmd->right)
			dup2(cmd->pipe[PIPE_IN], STDOUT_FILENO);
		run_redirect_in(sh, cmd);
		errno = 0;
		ret = execve(path, cmd->argv, envp);
		if (errno != 0)
			ft_fprintf(STDERR, "AShellM: %s: %s\n", path, strerror(errno));
		return (ret);
	}
	return (-1);
}

int		test_dir(char *path, char *cmd)
{
	struct dirent	*file;
	DIR				*rep;
	int				ret;

	file = NULL;
	rep = NULL;
	if ((rep = opendir(path)) == NULL)
		return (-1);
	while ((file = readdir(rep)) != NULL)
	{
		if (!ft_strncmp(file->d_name, cmd, ft_strlen(cmd) + 1))
		{
			if (file->d_type != 8 && file->d_type != 10)
				ret = 0;
			else
				ret = 1;
			if (closedir(rep) == -1)
				return (-1);
			return (1);
		}
	}
	if (closedir(rep) == -1)
		return (-1);
	return (0);
}

int		ft_inset(char *str, char c)
{
	int		i;

	i = 0;
	while (*(str + i))
		if (*(str + i++) == c)
			return (--i);
	return (-1);
}

void
	sigint_void(int sig)
{
	write(1, "\r", 1);
	return;
}

int		fork_exec(t_shell *sh, t_cmd *cmd, char *tmp[2], int nb)
{
	pid_t	child;
	char	**envp;
	char	*bin_path;
	char	*res;
	int		ret;

	child = -1;
	envp = convert_env_list(sh->env);
	ret = 0;
	if (nb == 2)
		bin_path = ft_strmjoin(2, tmp, "/");
	else if (nb == 1)
		bin_path = ft_strdup(tmp[0]);
	child = fork();
	signal(SIGINT, sigint_void);
	if (child == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_DFL);
	}
	tcsetattr(1, 0, &sh->term.old_term);
	ret = try_exec(sh, bin_path, cmd, envp, child);
	signal(SIGINT, sigint_quit);
	tcsetattr(1, 0, &sh->term.term);
	if (child == 0 && ret != 0)
		exit(ret);
	free(bin_path);
	free_env_array(envp);
	return (ret);
}


int		exec_bin(t_shell *sh, t_cmd *cmd)
{
	char	**paths;
	char	*tmp[2];
	int		i;
	int		ret;

	paths = ft_split(get_value(sh->env, "PATH", NULL), ':');
	if ((ret = ft_inset(cmd->argv[0], '/') != -1))
	{
		if (ret == 1)
			tmp[0] = cmd->argv[0];
		ret = fork_exec(sh, cmd, tmp, 1);
	}
	else if (paths)
	{
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
		ret = fork_exec(sh, cmd, tmp, 2);
	}
	if (paths)
		free_env_array(paths);
	return (1);
}
