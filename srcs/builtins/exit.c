/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 01:56:48 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 02:45:40 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_prog(t_shell *sh, t_cmd *cmd)
{
	sh->stop = 1;
	if (cmd->argc > 1)
	{
		if (ft_str_isnumeric(cmd->argv[1]))
		{
			if (cmd->argc == 2)
				sh->exit_val = ft_atoi(cmd->argv[1]);
			else
			{
				sh->stop = 0;
				ft_fprintf(STDERR, MSG_ERROR, "exit: too many arguments");
			}
		}
		else
		{
			ft_fprintf(STDERR, MSG_ERROR, "exit value should be numeric");
			sh->exit_val = 2;
		}
	}
	else
		sh->exit_val = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
