/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/04 01:00:45 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

__attribute__((destructor)) void quit(void)
{
	system("leaks minishell");
}

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

int
	main(int ac, char **av, char **envp)
{
	t_shell sh;

	(void)ac;
	(void)av;
	sh = (t_shell) {
		.input = NULL, .dir = "",
		.stop = 0, .cmds = NULL,
		.printed_dir = "",
		.env = create_env_list(envp)
	};
	getcwd(sh.dir, BUFFER_SIZE);
	prompt_line(&sh);
	free_env_list(&sh.env);
	ft_lstclear(&sh.cmds, free_command);
	return (0);
}
