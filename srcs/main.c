/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/09 17:33:14 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <term.h>
#include <ncurses.h>

t_shell	g_sh;
int		g_termx = 0;
int		g_termy = 0;

__attribute__((destructor)) void lul(void)
{
	system("leaks minishell");
}

void
	clear_last_prompt(t_shell *sh)
{
	ft_lstclear(&sh->cmds, free_command);
	ft_memdel((void **)&sh->input);
}

void
	err_shutdown(t_shell *sh, char *str)
{
	ft_fprintf(2, MSG_ERROR, str);
	clear_last_prompt(sh);
	exit(1);
}

void
	sigint_quit(int sig)
{
	(void)sig;
	format_directory(&g_sh);
	write(1, " \n", 2);
	ft_printf(MSG_PROMPT, g_sh.printed_dir);
	return ;
}

void
	handle_winch(int sig)
{
	(void)sig;
	format_directory(&g_sh);
	signal(SIGWINCH, SIG_IGN);
	initscr();
	refresh();
	g_termx = COLS;
	g_termy = LINES;
	endwin();
	signal(SIGWINCH, handle_winch);
}

int
	main(int ac, char **av, char **envp)
{
	struct termios	s_termios;
	struct termios	s_termios_backup;
	char			*tmp;

	(void)ac;
	(void)av;
	signal(SIGINT, sigint_quit);
	signal(SIGQUIT, SIG_IGN);
	g_sh = (t_shell) {
		.input = NULL, .dir = "",
		.stop = 0, .cmds = NULL,
		.printed_dir = "", .last_ret = 0,
		.env = create_env_list(envp)
	};
	tmp = ft_itoa(ft_atoi((char *)get_value(g_sh.env, "SHLVL", "0")) + 1);
	set_value(&g_sh.env, "SHLVL", tmp);
	ft_memdel((void **)&tmp);
	if (init_term(&s_termios, &s_termios_backup) == 0)
	{
		handle_winch(0);
		g_sh.old_term = s_termios_backup;
		g_sh.term = s_termios;
		format_directory(&g_sh);
		prompt_line(&g_sh);
		free_env_list(&g_sh.env);
		ft_lstclear(&g_sh.cmds, free_command);
		if (tcsetattr(0, 0, &s_termios_backup) == -1)
			return (-1);
	}
	else
	if (1)
	{
		get_next_line(0, &g_sh.input);
		sanitize_input2(&g_sh);
		exec_lines(&g_sh);
		clear_last_prompt(&g_sh);
		free_env_list(&g_sh.env);
	}
	return (0);
}
