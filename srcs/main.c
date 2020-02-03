/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/03 19:02:20 by adda-sil         ###   ########.fr       */
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
	ft_printf("💩  \033[0;31m[Error]\033[0m %s\n", str);
	clear_last_prompt(sh);
	exit(1);
}

int
	main(void)
{
	t_shell sh;

	sh = (t_shell) { .input = NULL, .dir = "AShellM", .stop = 0, .cmds = NULL };
	prompt_line(&sh);
	system("leaks minishell");
	return (0);
}
