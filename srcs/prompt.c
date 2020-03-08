/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/01 20:53:37 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/08 15:15:28 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	ft_read(t_shell *sh)
{
	sh->term.size_prt = ft_strlen(sh->printed_dir) + 7;
	sh->input = read_input(sh);
	write(1, "\n", 1);
	return ((sh->input == (char *)-1) ? 0 : 1);
}

int
	prompt_line(t_shell *sh)
{
	ft_printf("\e[7m%%\e[0m%*s\r", g_sh.term.pos.x - 1, "");
	ft_printf(sh->last_ret == EXIT_SUCCESS ?
		MSG_PROMPT : MSG_PROMPT_ERR, sh->printed_dir);
	if (ft_read(sh) == 0)
		sh->stop = 1;
	if (!sh->stop)
	{
		if (sanitize(sh))
		{
			parse_input(sh);	
			if (!add_to_history(sh))
				ft_memdel((void **)&sh->input);
		}
	}
	else
		ft_memdel((void **)&sh->input);
	clear_last_prompt(sh);
	return (SUC);
}
