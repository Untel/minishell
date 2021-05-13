/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2021/05/13 02:53:45 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include "minishell.h"
#include "line_edit.h"

/*
** __attribute__((destructor)) void lul(void)
** {
** 	if (!g_sh.sub)
** 		system("leaks minishell");
** }
*/

int
	ft_read(t_shell *sh)
{
	char	*prompt;
	char	tmp[4096];

	prompt = NULL;
	ft_sprintf(tmp, sh->last_ret == EXIT_SUCCESS ?
		MSG_PROMPT : MSG_PROMPT_ERR, sh->printed_dir);
	append(&prompt, tmp);
	sh->input = read_input(sh, prompt, MULTI, ft_strlen(sh->printed_dir) + 3);
	ft_memdel((void **)&prompt);
	if (sh->input == (char*)-3)
	{
		sh->input = ft_strdup("");
		sh->last_ret = 130;
	}
	if (sh->input == ((char *)-1))
		sh->input = NULL;
	return (!!sh->input);
}

int
	prompt_line(t_shell *sh)
{
	if (!ft_read(sh))
		sh->stop = 1;
	if (!sh->stop)
	{
		if (sanitize(sh))
		{
			parse_input(sh);
			ft_memdel((void **)&sh->input);
		}
	}
	else
		ft_memdel((void **)&sh->input);
	clear_last_prompt(sh);
	return (SUC);
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

void
	initialize_shell(t_shell *sh)
{
	char	*tmp;
	int		shlvl;

	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	tmp = get_value(sh->env, "SHLVL", "0");
	shlvl = ft_str_isnumeric(tmp) ? ft_atoi((char *)tmp) : 0;
	shlvl = (shlvl < 0 ? 0 : shlvl + 1);
	if (!(tmp = ft_itoa(shlvl)))
		err_shutdown(sh, "Cannot upgrade sh_level");
	tcgetattr(0, &sh->term.term);
	set_value(&sh->env, "SHLVL", tmp);
	format_directory(sh);
	set_value(&sh->env, "PWD", sh->dir);
	if (!get_value(sh->env, "OLDPWD", NULL))
		set_value(&sh->env, "OLDPWD", NULL);
	set_value(&sh->env, "GREP_COLOR", "00;38;5;226");
	ft_memdel((void **)&tmp);
	sh->alias = load_alias(sh);
}

int
	main(int ac, char **av, char **envp)
{
	t_shell sh;

	sh = (t_shell) {
		.input = NULL, .dir = "", .stop = 0, .cmds = NULL,
		.printed_dir = "", .last_ret = 0, .hd_index = 0,
		.env = create_env_list(envp), .exit_val = EXIT_SUCCESS,
		.heredocs = NULL, .inline_fd = -1, .sub = 0,
		.cmd_history.filename = ".history",
	};
	load_history(sh.cmd_history.filename, (t_history *)&sh.cmd_history);
	initialize_shell(&sh);
	if (ac > 1)
		inline_mode(&sh, *(av + 1));
	else if (isatty(STDIN_FILENO))
	{
		while (!sh.stop)
			prompt_line(&sh);
		ft_printf(MSG_EXIT);
	}
	else
		inline_mode(&sh, NULL);
	free_env_list(&sh.env);
	free_history((t_history *)&sh.cmd_history, 0);
	free_alias(&sh);
	return (sh.exit_val);
}
