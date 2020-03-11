/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 16:21:25 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/11 05:24:04 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"

void
	handle_right_arrow(char buff[6], t_term *term)
{
	term->r += term->r_ofst < (term->pos.x -
			(term->size_prt % term->pos.x)) ? 1 : 0;
	if (term->r == 0 && term->l != 0)
		--term->l;
	else
		write(1, buff, 3 * (term->pos_str > 1));
	if (term->pos_str > 1)
		--term->pos_str;
	if (term->pos_aff > 1)
		--term->pos_aff;
}

void
	handle_left_arrow(char buff[6], t_term *term)
{
	if (term->pos_str <= term->input->size)
	{
		term->r -= term->r_ofst > 0 ? 1 : 0;
		if (term->r_ofst == 0 &&
				term->l + term->pos_aff <= term->input->size)
			++term->l;
		else if (term->r_ofst && term->pos_aff < term->input->size)
			++term->pos_aff;
		write(1, buff, 3 * (term->pos_aff < term->input->size));
		term->pos_str += term->pos_str < term->input->size ? 1 : 0;
	}
}

void
	handle_home(t_shell *sh, char buff[6], t_term *term)
{
	int i;

	(void)sh;
	i = -1;
	buff[2] = 'D';
	while (++i < term->input->size)
		handle_left_arrow(buff, term);
}

void
	handle_end(t_shell *sh, char buff[6], t_term *term)
{
	int i;

	(void)sh;
	i = -1;
	buff[2] = 'C';
	while (++i < term->input->size)
		handle_right_arrow(buff, term);
}
