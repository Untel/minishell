/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 18:47:45 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/23 23:41:26 by adda-sil         ###   ########.fr       */
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
	sigint_quit(int sig)
{
	(void)sig;
	format_directory(&g_sh);
	handle_ctrl_c(&g_sh.term);
	write(1, "\n", 1);
	ft_printf("\e[7m%%\e[0m%*s\r", g_sh.term.pos.x - 1, "");
	g_sh.term.size_prt = ft_strlen(g_sh.printed_dir) + 7;
	ft_printf(g_sh.last_ret == EXIT_SUCCESS ?
		MSG_PROMPT : MSG_PROMPT_ERR, g_sh.printed_dir);
	g_sh.ctrl_c = 1;
}

void
	handle_winch(int sig)
{
	char	**env;
	char	*av[3];

	(void)sig;
	env = convert_env_list(g_sh.env);
	av[0] = "tput";
	av[1] = "cols";
	av[2] = 0;
	g_sh.term.pos.x = get_termx(&g_sh, av, env);
	signal(SIGWINCH, handle_winch);
	free_env_array(env);
}
