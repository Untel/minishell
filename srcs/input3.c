/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 17:30:32 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/10 15:31:14 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	copy_to_cmd(t_shell *sh, t_read *rd, int *i)
{
	if (*i != rd->index)
		copy_from_idx(sh, rd, *i);
	if (rd->buffer)
	{
		if (!add_arg_to_last_cmd(sh, rd->buffer, rd))
			return (FALSE);
	}
	return (SUC);
}

int
	del_right(t_term *term)
{
	if (term->pos_str > 1)
	{
		delone(term->input, term->pos_str);
		term->pos_str -= 1;
		term->pos_aff -= 1;
		return (1);
	}
	return (0);
}
