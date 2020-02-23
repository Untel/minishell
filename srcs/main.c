/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/23 19:07:13 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// __attribute__((destructor)) void lul(void)
// {
// 	system("leaks minishell");
// }

t_shell	g_sh;

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
	initialize_shell(t_shell *sh)
{
	char	*tmp;

	signal(SIGINT, sigint_quit);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (!(tmp = ft_itoa(ft_atoi((char *)get_value(sh->env, "SHLVL", "0")) + 1)))
		err_shutdown(sh, "Cannot upgrade sh_level");
	set_value(&sh->env, "SHLVL", tmp);
	set_value(&sh->env, "GREP_OPTIONS", "--color=auto");
	set_value(&sh->env, "GREP_COLOR", "00;38;5;226");
	ft_memdel((void **)&tmp);
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
	};
	initialize_shell(&g_sh);
	if (init_term(&g_sh.term.term, &g_sh.term.old_term) == 0)
	{
		handle_winch(0);
		format_directory(&g_sh);
		prompt_line(&g_sh);
		ft_lstclear(&g_sh.cmds, free_command);
		tcsetattr(0, 0, &g_sh.term.old_term);
	}
	else
	{
		get_next_line(0, &g_sh.input);
		if (sanitize(&g_sh) && parse_input(&g_sh))
			exec_lines(&g_sh);
		clear_last_prompt(&g_sh);
	}
	free_env_list(&g_sh.env);
	return (EXIT_SUCCESS);
}
