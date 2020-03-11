/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_cursor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adda-sil <adda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 17:26:09 by adda-sil          #+#    #+#             */
/*   Updated: 2020/03/11 21:30:27 by adda-sil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int
	is_first_letter(t_term *term, int pos)
{
	char	c;
	char	cc;
	int		is;

	if (pos >= term->input->size)
		return (1);
	c = get_elem_by_pos(term->input, pos - 1)->c;
	cc = get_elem_by_pos(term->input, pos)->c;
	is = ft_isalnum(c) && !ft_isalnum(cc);
	return (is);
}

void
	move_next_word(t_term *term)
{
	static char a[6] = { 27, 91, 'C', 0, 0 };

	handle_right_arrow(a, term);
	while ((term->pos_str > 1)
		&& !is_first_letter(term, term->pos_str))
		handle_right_arrow(a, term);
}

void
	move_prev_word(t_term *term)
{
	static char a[6] = { 27, 91, 'D', 0, 0 };

	handle_left_arrow(a, term);
	while ((term->pos_str <= term->input->size)
		&& !is_first_letter(term, term->pos_str))
		handle_left_arrow(a, term);
}

void
	handle_ctrl_keys(char buff[6], t_term *term)
{
	if (buff[3] == 59 && buff[4] == 53)
	{
		if (buff[5] == 67)
			move_next_word(term);
		else if (buff[5] == 68)
			move_prev_word(term);
	}
}
