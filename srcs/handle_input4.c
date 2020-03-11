/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 19:11:56 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/11 05:18:58 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	input_copy(t_shell *sh)
{
	ft_memdel((void **)&sh->term.clipboard);
	sh->term.clipboard = copy_str_at(sh->term.input,
		sh->term.input->size - sh->term.pos_str);
}

void
	input_cut(t_shell *sh)
{
	int i;
	int	l;

	i = sh->term.input->size - sh->term.pos_str - 1;
	l = sh->term.input->size;
	input_copy(sh);
	while (++i < l)
	{
		write(1, " ", 1);
		del_right(&sh->term);
	}
}

void
	input_past(t_shell *sh)
{
	int i;

	i = 0;
	while (sh->term.clipboard[i])
	{
		add_after(sh->term.input,
			sh->term.clipboard[i], sh->term.pos_str);
		i++;
	}
}

void
	handle_option_cases(t_shell *sh, char b[6])
{
	read(0, b + 1, 5);
	if (b[0] == -61 && b[1] == -89)
		input_copy(sh);
	else if (b[0] == -30 && b[1] == -120 && b[2] == -102)
		input_past(sh);
	else if (b[0] == -30 && b[1] == -119 && b[2] == -120)
		input_cut(sh);
}
