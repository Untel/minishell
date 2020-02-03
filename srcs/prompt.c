/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:53:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/02 22:08:58 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	prompt_line(t_shell *sh)
{
	getcwd(sh->dir, BUFFER_SIZE);
	ft_printf("🔥 \033[0;32m%s\033[0m$ ", sh->dir);
	get_next_line(0, &(sh->input));
	sanitize_input(sh);
	if (ft_strncmp("exit", sh->input, 5) == 0)
		sh->stop = 1;
	free(sh->input);
	if (sh->stop)
		ft_printf("🖐  \033[0;31mGood bye!\033[0m\n");
	else
		prompt_line(sh);
	return (SUC);
}
