/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 20:24:06 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/21 16:05:25 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	ft_echo(t_shell *sh, t_cmd *cmd)
{
	int	i;
	int	last_line;

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

int
	ft_pwd(t_shell *sh, t_cmd *cmd)
{
	if (cmd->argc > 1)
	{
		ft_fprintf(STDERR, MSG_ERROR, "pwd: too many arguments");
		return (EXIT_FAILURE);
	}
	write(STDOUT_FILENO, sh->dir, ft_strlen(sh->dir));
	write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}
