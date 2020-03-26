/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/26 15:56:06 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	g_sh;

/*
** __attribute__((destructor)) void lul(void)
** {
** 	if (!g_sh.sub)
** 		system("leaks minishell");
** }
*/

void
	initialize_shell(t_shell *sh)
{
	char	*tmp;
	int		shlvl;

	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	tmp = get_value(sh->env, "SHLVL", "0");
	shlvl = ft_str_isnumeric(tmp) ? ft_atoi((char *)tmp) : 0;
	shlvl = (shlvl < 0 ? 0 : shlvl + 1);
	if (!(tmp = ft_itoa(shlvl)))
		err_shutdown(sh, "Cannot upgrade sh_level");
	set_value(&sh->env, "SHLVL", tmp);
	format_directory(sh);
	set_value(&sh->env, "PWD", sh->dir);
	set_value(&sh->env, "OLDPWD", NULL);
	set_value(&sh->env, "GREP_OPTIONS", "--color=auto");
	set_value(&sh->env, "GREP_COLOR", "00;38;5;226");
	ft_memdel((void **)&tmp);
}

void
	run(t_shell *sh)
{
	format_directory(sh);
	while (!sh->stop)
		prompt_line(sh);
	ft_printf(MSG_EXIT);
}

int
	inline_mode(t_shell *sh, char *filename)
{
	int		ret;

	if (filename)
	{
		errno = 0;
		if ((sh->inline_fd = open(filename, O_RDONLY)) == ERR)
			return (ft_fprintf(STDERR, MSG_GEN_ERR, filename,
				errno ? strerror(errno) : "no such file or directory"));
	}
	else
		sh->inline_fd = STDIN_FILENO;
	if (sh->inline_fd != ERR)
		while ((ret = get_next_line(sh->inline_fd, &sh->input)) >= 0)
		{
			if (sanitize(sh))
				parse_input(sh);
			ft_memdel((void **)&sh->input);
			clear_last_prompt(sh);
			if (ret == 0)
				break ;
		}
	if (sh->inline_fd != STDIN_FILENO)
		close(sh->inline_fd);
	return (SUC);
}

int
	main(int ac, char **av, char **envp)
{
	g_sh = (t_shell) {
		.input = NULL, .dir = "", .stop = 0, .cmds = NULL,
		.printed_dir = "", .last_ret = 0, .hd_index = 0,
		.env = create_env_list(envp),
		.heredocs = NULL, .inline_fd = -1, .sub = 0,
	};
	initialize_shell(&g_sh);
	if (ac > 1)
		inline_mode(&g_sh, *(av + 1));
	else
		run(&g_sh);
	free_env_list(&g_sh.env);
	return (EXIT_SUCCESS);
}
