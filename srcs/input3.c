/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 17:30:32 by adda-sil          #+#    #+#             */
/*   Updated: 2020/02/27 17:31:17 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	copy_to_cmd(t_shell *sh, t_read *rd, int *i)
{
	if (*i != rd->index)
		copy_from_idx(sh, rd, *i);
	if (rd->buffer)
		add_arg_to_last_cmd(sh, rd->buffer, rd);
	return (SUC);
}
