/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:27:15 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/02 21:51:31 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	err_shutdown(char *str)
{
	ft_printf("💩  \033[0;31m[Error]\033[0m %s\n", str);
	exit(1);
}

int
	main(void)
{
	t_shell sh;

	sh = (t_shell) { .input = NULL, .dir = "AShellM", .stop = 0, .cmds = NULL };
	prompt_line(&sh);
	return (0);
}