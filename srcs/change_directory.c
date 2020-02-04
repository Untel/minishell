/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:57:27 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 05:19:55 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	format_directory(t_shell *sh)
{
	getcwd(sh->dir, BUFFER_SIZE);
	ft_strcpy(sh->printed_dir, sh->dir);
	ft_strreplace(sh->printed_dir, get_value(sh->env, "HOME") + 1, "🏡 ");
	ft_strreplace(sh->printed_dir, ".Trash", "🗑 ");
	ft_strreplace(sh->printed_dir, "usr", "👤 ");
	ft_strreplace(sh->printed_dir, "bin", "🍆 ");
}

int
	change_directory(t_shell *sh, t_cmd *cmd)
{
	(void)sh;
	int ret;
	char *dir;

	if (cmd->argc == 1)
		dir = get_value(sh->env, "HOME");
	else
		dir = !ft_strncmp(cmd->argv[1], "-", 2)
			? get_value(sh->env, "OLDPWD") : cmd->argv[1];
	ret = chdir(dir);
	if (ret == ERR)
		ft_printf(MSG_ERROR, strerror(errno));
	else
	{
		set_value(&sh->env, "OLDPWD", sh->dir);
		format_directory(sh);
	}
	return (ret != ERR);
}
