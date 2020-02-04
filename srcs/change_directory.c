/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:57:27 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 01:00:07 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	change_directory(t_shell *sh, t_cmd *cmd)
{
	(void)sh;
	int ret;
	char *dir;

	if (cmd->argc == 1)
		dir = get_value(sh->env, "HOME");
	else
		dir = !ft_strncmp(cmd->argv[1], "-", ft_strlen(cmd->argv[1]))
			? get_value(sh->env, "OLDPWD") : cmd->argv[1];
	ret = chdir(dir);
	if (ret == ERR)
		ft_printf(MSG_ERROR, strerror(errno));
	else
	{
		set_value(&sh->env, "OLDPWD", sh->dir);
		getcwd(sh->dir, BUFFER_SIZE);
	}
	return (ret != ERR);
}
