/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: riblanc <riblanc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/13 01:57:39 by riblanc           #+#    #+#             */
/*   Updated: 2021/05/13 01:57:50 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
