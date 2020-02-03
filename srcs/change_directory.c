/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:57:27 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 22:53:37 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	change_directory(t_shell *sh, t_cmd *cmd)
{
	(void)sh;
	int ret;
	char *dir;

	if (ft_strncmp(cmd->argv[1], "-", ft_strlen(cmd->argv[1])))
		dir = get_value(sh->env, "OLDPWD");
	else
		dir = cmd->argv[1]
	ret = chdir(dir);
	if (ret == ERR)
		ft_printf(MSG_ERROR, strerror(errno));
	else
		getcwd(sh->dir, BUFFER_SIZE);
	return (ret != ERR);
}