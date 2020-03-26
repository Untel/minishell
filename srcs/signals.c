/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 18:47:45 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/26 15:55:15 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	sigint_void(int sig)
{
	(void)sig;
	write(1, "\r", 1);
}

void
	init_child_signals(pid_t child)
{
	signal(SIGINT, sigint_void);
	if (child == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_DFL);
	}
}
