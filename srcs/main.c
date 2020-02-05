/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/05 17:27:21 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	clear_last_prompt(t_shell *sh)
{
	ft_lstclear(&sh->cmds, free_command);
	free(sh->input);
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
	char	buf[BUFFER_SIZE];

	getcwd(buf, BUFFER_SIZE);
	write(1, " \n", 2);
	ft_printf(MSG_PROMPT, buf);
	return ;
}

int
	main(int ac, char **av, char **envp)
{
	t_shell sh;
	char	*tmp;

	(void)ac;
	(void)av;
	signal(SIGINT, quit);
//	signal(SIGQUIT, quit);
	sh = (t_shell) {
		.input = NULL, .dir = "",
		.stop = 0, .cmds = NULL,
		.printed_dir = "", .last_ret = 0,
		.env = create_env_list(envp)
	};
	tmp = ft_itoa(ft_atoi((char *)get_value(sh.env, "SHLVL", "0")) + 1);
	set_value(&sh.env, "SHLVL", tmp);
	free(tmp);
	tmp = NULL;
	format_directory(&sh);
	prompt_line(&sh);
	free_env_list(&sh.env);
	ft_lstclear(&sh.cmds, free_command);
	return (0);
}
