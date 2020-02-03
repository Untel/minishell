/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_directory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 20:57:27 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 21:21:21 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	change_directory(t_shell *sh, t_cmd *cmd)
{
	(void)sh;
	int ret;

	ret = chdir(cmd->argv[1]);
	if (ret == ERR)
		ft_printf(MSG_ERROR, strerror(errno));
	return (ret != ERR);
}