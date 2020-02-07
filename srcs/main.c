/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/07 21:58:47 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <term.h>
#include <ncurses.h>
t_shell			sh;

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
	ft_printf(MSG_ERROR, str);
	clear_last_prompt(sh);
	exit(1);
}

int		g_ctrl_c = 0;

void
	quit (int sig)
{
	// char	buf[BUFFER_SIZE];

	format_directory(&sh);
	// getcwd(buf, BUFFER_SIZE);
	// write(1, " \n", 2);
	// ft_printf(MSG_PROMPT, buf);
	// prompt_line(&sh);
	return ;
}

int
	main(int ac, char **av, char **envp)
{
	struct termios	s_termios;
	struct termios	s_termios_backup;
	char			*tmp;

	(void)ac;
	(void)av;
	if (init_term(&s_termios, &s_termios_backup) == 0)
	{
		signal(SIGINT, quit);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		sh = (t_shell) {
			.input = NULL, .dir = "",
			.stop = 0, .cmds = NULL,
			.printed_dir = "", .last_ret = 0,
			.env = create_env_list(envp)
		};
		tmp = ft_itoa(ft_atoi((char *)get_value(sh.env, "SHLVL", "0")) + 1);
		set_value(&sh.env, "SHLVL", tmp);
		ft_memdel((void **)&tmp);
		format_directory(&sh);
		prompt_line(&sh);
		free_env_list(&sh.env);
		ft_lstclear(&sh.cmds, free_command);
		if (tcsetattr(0, 0, &s_termios_backup) == -1)
			return (-1);
	}
	return (0);
}
