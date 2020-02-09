/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:57:27 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/09 09:02:34 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	format_directory(t_shell *sh)
{
	char *home;

	getcwd(sh->dir, BUFFER_SIZE);
	ft_strcpy(sh->printed_dir, sh->dir);
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
			return (ft_fprintf(2, MSG_ERROR, "no env HOME defined") && 0);
	}
	else
		dir = !ft_strncmp(cmd->argv[1], "-", 2)
			? get_value(sh->env, "OLDPWD", sh->dir) : cmd->argv[1];
	ret = chdir(dir);
	if (ret == ERR)
		ft_fprintf(2, MSG_ERROR, strerror(errno));
	else
	{
		set_value(&sh->env, "OLDPWD", sh->dir);
		format_directory(sh);
	}
	return (ret != ERR);
}
