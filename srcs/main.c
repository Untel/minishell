/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/16 20:33:20 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <term.h>
#include <ncurses.h>

t_shell	g_sh;
int		g_termx = 0;
int		g_termy = 0;

// __attribute__((destructor)) void lul(void)
// {
//	system("leaks minishell");
// }

void
	clear_last_prompt(t_shell *sh)
{
	ft_lstclear(&sh->cmds, free_command);
	ft_memdel((void **)&sh->input);
}

void
	err_shutdown(t_shell *sh, char *str)
{
	ft_fprintf(STDERR, MSG_ERROR, str);
	clear_last_prompt(sh);
	exit(1);
}

void
	sigint_quit(int sig)
{
	(void)sig;
	format_directory(&g_sh);
	handle_ctrl_c(&g_sh.term);
	write(1, "\n", 1);
	ft_printf("\e[7m%%\e[0m%*s\r", g_termx - 1, "");
	ft_printf(MSG_PROMPT, g_sh.printed_dir);
	g_sh.ctrl_c = 1;
	return ;
}

int
	get_termx(t_shell *sh, char **av, char **env)
{
	int		pid;
	int		p[2];
	char	str[11];
	int		ret;
	int		res;

	pipe(p);
	res = 0;
	if ((pid = fork()) == -1)
		return (-1);
	else if (pid == 0)
	{
		dup2(p[1], 1);
		close(p[0]);
		ret = execve("/usr/bin/tput", av, env);
		exit(ret);
	}
	else
	{
		close(p[1]);
		wait(0);
		ret = read(p[0], str, 10);
		str[ret] = 0;
		res = ft_atoi(str);
	}
	return (res);
}

void
	handle_winch(int sig)
{
	char	**env;
	char	*av[3];

	env = convert_env_list(g_sh.env);
	av[0] = "tput";
	av[1] = "cols";
	av[2] = 0;
	g_termx = get_termx(&g_sh, av, env);
	signal(SIGWINCH, handle_winch);
	free_env_array(env);
}

int
	main(int ac, char **av, char **envp)
{
	char			*tmp;

	(void)ac;
	(void)av;
	signal(SIGINT, sigint_quit);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	g_sh = (t_shell) {
		.input = NULL, .dir = "",
		.stop = 0, .cmds = NULL,
		.printed_dir = "", .last_ret = 0,
		.env = create_env_list(envp),
		.ctrl_c = 0
	};
	tmp = ft_itoa(ft_atoi((char *)get_value(g_sh.env, "SHLVL", "0")) + 1);
	set_value(&g_sh.env, "SHLVL", tmp);
	set_value(&g_sh.env, "GREP_OPTIONS", "--color=auto");
	set_value(&g_sh.env, "GREP_COLOR", "00;38;5;226");
	ft_memdel((void **)&tmp);
	if (init_term(&g_sh.term.term, &g_sh.term.old_term) == 0)
	{
		handle_winch(0);
		format_directory(&g_sh);
		prompt_line(&g_sh);
		free_env_list(&g_sh.env);
		ft_lstclear(&g_sh.cmds, free_command);
		if (tcsetattr(0, 0, &g_sh.term.old_term) == -1)
			return (-1);
	}
	else
	{
		get_next_line(0, &g_sh.input);
		sanitize_input2(&g_sh);
		exec_lines(&g_sh);
		clear_last_prompt(&g_sh);
		free_env_list(&g_sh.env);
	}
	return (0);
}
