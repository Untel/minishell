/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 20:24:06 by adda-sil          #+#    #+#             */
/*   Updated: 2021/05/13 01:57:59 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	ft_echo(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	last_line;

	(void)sh;
	i = 0;
	last_line = 1;
	while (++i < cmd->argc && ft_strcmp(cmd->argv[i], "-n") == 0)
		last_line = 0;
	while (i < cmd->argc)
	{
		write(STDOUT_FILENO, cmd->argv[i], ft_strlen(cmd->argv[i]));
		if (i != cmd->argc - 1)
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (last_line)
		write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}
