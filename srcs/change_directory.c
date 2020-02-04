/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:57:27 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 18:28:35 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	format_directory(t_shell *sh)
{
	char *home;
	getcwd(sh->dir, BUFFER_SIZE);
	ft_strcpy(sh->printed_dir, sh->dir);
	home = get_value(sh->env, "HOME", NULL);
	if (home)
		ft_strreplace(sh->printed_dir, home + 1, "🏡 ");
	ft_strreplace(sh->printed_dir, ".Trash", "🗑 ");
	ft_strreplace(sh->printed_dir, "usr", "👤 ");
	ft_strreplace(sh->printed_dir, "bin", "🍆 ");
}

int
	change_directory(t_shell *sh, t_cmd *cmd)
{
	int ret;
	char *dir;
	char *old;

	if (cmd->argc == 1)
	{
		dir = get_value(sh->env, "HOME", NULL);
		if (!dir)
		{
			ft_printf(MSG_ERROR, "no env HOME defined");
			return (0);
		}
	}
	else if (!ft_strncmp(cmd->argv[1], "-", 2))
		dir = get_value(sh->env, "OLDPWD", sh->dir);
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
