/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:07:50 by riblanc           #+#    #+#             */
/*   Updated: 2020/02/26 19:10:42 by riblanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"

void
	handle_arrows(char buff[3], t_term *term)
{
	read(0, buff + 1, 1);
	read(0, buff + 2, 1);
	if (!(buff[1] == '[' && buff[2] == 'A') &&
			!(buff[1] == '[' && buff[2] == 'B'))
	{
		if (buff[1] == '[' && buff[2] == 'C')
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
		else if (buff[1] == '[' && buff[2] == 'D')
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
	}
	if (buff[1] == '[' && buff[2] == 'A')
		print_history(&g_sh, 1);
	else if (buff[1] == '[' && buff[2] == 'B')
		print_history(&g_sh, 0);
}

void
	handle_backspace(char buff[3], t_term *term)
{
	if (term->input->size > 1 && term->pos_str < term->input->size)
		delone(term->input, term->pos_str + 1);
	if (term->l > 0)
	{
		if (term->l_ofst <= 0)
		{
			++term->pos_aff;
			--term->r;
		}
		else
			++term->l;
	}
	write(1, "\e[D", 3);
	write(1, buff, 1);
	write(1, "\e[D", 3);
}

int
	handle_ctrl_d(char buff[3], t_term *term)
{
	if (term->input->size == 1)
	{
		free_all(term->input);
		ft_memdel((void **)&term->input);
		return (-1);
	}
	if (term->pos_str > 1)
	{
		delone(term->input, term->pos_str);
		term->pos_str -= 1;
		term->pos_aff -= 1;
	}
	write(1, buff, 1);
	return (1);
}

void
	handle_ctrl_u(t_term term)
{
	int		i;
	char	del;

	del = 127;
	i = -1;
	while (++i <= term.input->size - term.pos_aff)
		write(1, "\e[D", 3);
	i = -1;
	while (++i < term.input->size)
		write(1, &del, 1);
	while (term.input->size > 1 && term.pos_str < term.input->size)
		delone(term.input, term.pos_str + 1);
}

void
	handle_ctrl_c(t_term *term)
{
	int		i;
	char	del;

	del = 127;
	i = -1;
	while (++i <= term->input->size - term->pos_aff)
		write(1, "\e[D", 3);
	i = -1;
	while (++i < term->input->size)
		write(1, &del, 1);
	free_all(term->input);
	term->pos_str = 1;
	term->pos_aff = 1;
	add_empty(term->input, 0);
}
