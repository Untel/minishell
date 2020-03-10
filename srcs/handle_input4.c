/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 19:11:56 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/10 19:45:47 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void
	input_copy(t_shell *sh)
{
	// free_all(sh->term.clipboard);
	ft_fprintf(2, "%d %d | %d\n", sh->term.pos_str, sh->term.input->size, sh->term.input->size - sh->term.pos_str);
	sh->term.clipboard = copy_at(sh->term.input, sh->term.input->size - sh->term.pos_str);
	
	// affiche_inv(sh->term.clipboard, 20, 50);
}

void
	input_cut(t_shell *sh)
{
	(void)sh;
	ft_fprintf(2, "CUT\n");
}

void
	input_past(t_shell *sh)
{
	(void)sh;
	ft_fprintf(2, "PAST\n");
}

void
	handle_option_cases(t_shell *sh, char b[6])
{
	if (b[0] == -61 && b[1] == -89)
		input_copy(sh);
	else if (b[0] == -30 && b[1] == -120 && b[2] == -102)
		input_past(sh);
	else if (b[0] == -30 && b[1] == -119 && b[2] == -120)
		input_cut(sh);
}
