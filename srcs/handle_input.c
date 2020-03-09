/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/09 09:07:50 by riblanc           #+#    #+#             */
/*   Updated: 2020/03/09 23:29:22 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "minishell.h"

void
	handle_arrows(char buff[3], t_term *term)
{
	read(0, buff + 1, 1);
	read(0, buff + 2, 1);
	if (buff[1] == '[' && buff[2] == 'C')
		handle_right_arrow(buff, term);
	else if (buff[1] == '[' && buff[2] == 'D')
		handle_left_arrow(buff, term);
	else if (buff[1] == '[' && buff[2] == 'A')
		print_history(&g_sh, 1);
	else if (buff[1] == '[' && buff[2] == 'B')
		print_history(&g_sh, 0);
	else if (buff[1] == '[' && buff[2] == 'H')
		handle_home(&g_sh, buff, term);
	else if (buff[1] == '[' && buff[2] == 'F')
		handle_end(&g_sh, buff, term);
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
