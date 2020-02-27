/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/27 17:25:55 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	g_sh;

void
	initialize_shell(t_shell *sh)
{
	char	*tmp;

	signal(SIGINT, sigint_quit);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (!(tmp = ft_itoa(ft_atoi(
		(char *)get_value(sh->env, "SHLVL", "0")) + 1)))
		err_shutdown(sh, "Cannot upgrade sh_level");
	set_value(&sh->env, "SHLVL", tmp);
	set_value(&sh->env, "GREP_OPTIONS", "--color=auto");
	set_value(&sh->env, "GREP_COLOR", "00;38;5;226");
	ft_memdel((void **)&tmp);
	init_history(sh);
}

void
	run(t_shell *sh)
{
	handle_winch(0);
	format_directory(sh);
	while (!sh->stop)
		prompt_line(sh);
	ft_printf(MSG_EXIT);
	tcsetattr(0, 0, &sh->term.old_term);
}

void
	inline_mode(t_shell *sh)
{
	int	ret;
	while ((ret = get_next_line(0, &sh->input)) >= 0)
	{
		if (sanitize(sh))
			parse_input(sh);
		ft_memdel((void **)&sh->input);
		clear_last_prompt(sh);
		if (ret == 0)
			break;
	}
}

int
	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	g_sh = (t_shell) {
		.input = NULL, .dir = "", .stop = 0, .cmds = NULL,
		.printed_dir = "", .last_ret = 0, .hd_index = 0,
		.env = create_env_list(envp), .ctrl_c = 0,
		.heredocs = NULL,
	};
	initialize_shell(&g_sh);
	if (init_term(&g_sh.term.term, &g_sh.term.old_term) == 0)
		run(&g_sh);
	else
		inline_mode(&g_sh);
	persist_history(&g_sh);
	free_env_list(&g_sh.env);
	return (EXIT_SUCCESS);
}
