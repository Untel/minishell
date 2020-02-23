/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:57:27 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/23 17:43:21 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	format_directory(t_shell *sh)
{
	getcwd(sh->dir, BUFFER_SIZE);
	ft_strcpy(sh->printed_dir, sh->dir);
	return (SUC);
}

int
	change_directory(t_shell *sh, t_cmd *cmd)
{
	int		ret;
	char	*dir;
	char	*old;

	if (cmd->argc == 1)
	{
		if (ft_strncmp(cmd->argv[0], "cd", 3))
			dir = cmd->argv[0];
		else if (!(dir = get_value(sh->env, "HOME", NULL)))
			return (ft_fprintf(STDERR, MSG_ERROR, "no env HOME defined") && 0);
	}
	else
		dir = !ft_strncmp(cmd->argv[1], "-", 2)
			? get_value(sh->env, "OLDPWD", sh->dir) : cmd->argv[1];
	ret = chdir(dir);
	if (ret == ERR)
		ft_fprintf(STDERR, MSG_ERROR, strerror(errno));
	else
	{
		set_value(&sh->env, "OLDPWD", sh->dir);
		format_directory(sh);
	}
	return (ret != ERR ? 0 : ERR);
}
