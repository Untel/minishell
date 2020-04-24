/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 18:47:45 by adda-sil          #+#    #+#             */
/*   Updated: 2020/04/24 11:07:45 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	init_child_signals(pid_t child)
{
	if (child == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_DFL);
	}
}
